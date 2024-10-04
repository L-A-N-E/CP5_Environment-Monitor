import dash
from dash import dcc, html
from dash.dependencies import Input, Output, State
import plotly.graph_objs as go
import requests
from datetime import datetime
import pytz
from typing import Dict, Any, List

# Constantes para o IP e porta
IP_ADDRESS = "PUBLIC_IP"
PORT_STH = 8666
DASH_HOST = "0.0.0.0"

# Carregar estilos CSS
app = dash.Dash(__name__, external_stylesheets=['/assets/styles.css'])

def get_environmental_data(lastN: int) -> Dict[str, Any]:
    """
    Obtém dados ambientais de um monitor.

    :param lastN: Número de registros a serem obtidos.
    :return: Dicionário com os dados das métricas (luminosidade, temperatura, umidade).
    """
    metrics = ['luminosity', 'temperature', 'humidity']  # Métricas a serem buscadas
    data = {}

    for metric in metrics:
        # Construindo a URL para requisição dos dados
        url = f"http://{IP_ADDRESS}:{PORT_STH}/STH/v1/contextEntities/type/EnvironmentMonitor/id/urn:ngsi-ld:EnvironmentMonitor:001/attributes/{metric}?lastN={lastN}"
        headers = {
            'fiware-service': 'smart',
            'fiware-servicepath': '/'
        }
        response = requests.get(url, headers=headers)  # Realizando a requisição
        if response.status_code == 200:
            try:
                # Extraindo os valores da resposta JSON
                values = response.json()['contextResponses'][0]['contextElement']['attributes'][0]['values']
                data[metric] = values
            except KeyError as e:
                print(f"Key error: {e}")  # Exibindo erro se a chave não for encontrada
                data[metric] = []
        else:
            print(f"Error accessing {url}: {response.status_code}")  # Exibindo erro de requisição
            data[metric] = []

    return data  # Retornando os dados coletados

def convert_to_brazil_time(timestamps: List[str]) -> List[datetime]:
    """
    Converte timestamps de UTC para o horário de Brasília.

    :param timestamps: Lista de timestamps em formato UTC.
    :return: Lista de timestamps convertidos para o horário de Brasília.
    """
    utc = pytz.utc
    brasil = pytz.timezone('America/Sao_Paulo')
    converted_timestamps = []

    for timestamp in timestamps:
        try:
            # Formatação do timestamp
            timestamp = timestamp.replace('T', ' ').replace('Z', '')
            converted_time = utc.localize(datetime.strptime(timestamp, '%Y-%m-%d %H:%M:%S.%f')).astimezone(brasil)
        except ValueError:
            converted_time = utc.localize(datetime.strptime(timestamp, '%Y-%m-%d %H:%M:%S')).astimezone(brasil)
        converted_timestamps.append(converted_time)  # Adicionando o timestamp convertido

    return converted_timestamps  # Retornando os timestamps convertidos

lastN = 10  # Número de últimos dados a serem exibidos

app.layout = html.Div(children=[
    html.H1(children='Environmental Data Viewer'),  # Título do gráfico

    dcc.RadioItems(
        id='data-selector',
        options=[
            {'label': 'All', 'value': 'all'},
            {'label': 'Luminosity', 'value': 'luminosity'},
            {'label': 'Temperature', 'value': 'temperature'},
            {'label': 'Humidity', 'value': 'humidity'}
        ],
        value='luminosity',  # Valor padrão
        labelStyle={'display': 'inline-block', 'margin-right': '20px'},
        className='radio-items'
    ),

    dcc.Graph(id='environment-graph', className='graph-container'),  # Gráfico para exibir os dados ambientais

    dcc.Store(id='environment-data-store', data={'timestamps': [], 'luminosity': [], 'temperature': [], 'humidity': []}),  # Armazenamento dos dados ambientais

    dcc.Interval(
        id='interval-component',
        interval=10 * 1000,  # Intervalo de atualização de 10 segundos
        n_intervals=0
    )
], className='container')

@app.callback(
    Output('environment-data-store', 'data'),  # Atualizando o armazenamento dos dados
    Input('interval-component', 'n_intervals'),  # Intervalo de atualização
    State('environment-data-store', 'data')  # Estado atual do armazenamento
)
def update_data_store(n: int, stored_data: Dict[str, List[Any]]) -> Dict[str, List[Any]]:
    """
    Atualiza os dados armazenados a cada intervalo de tempo.

    :param n: Número de intervalos desde a última atualização.
    :param stored_data: Dados atualmente armazenados.
    :return: Dados atualizados armazenados.
    """
    data_environment = get_environmental_data(lastN)  # Obtendo os dados ambientais

    if data_environment:
        # Extraindo valores de luminosidade, temperatura e umidade
        luminosity_values = [float(entry['attrValue']) for entry in data_environment['luminosity']]
        temperature_values = [float(entry['attrValue']) for entry in data_environment['temperature']]
        humidity_values = [float(entry['attrValue']) for entry in data_environment['humidity']]
        timestamps = [entry['recvTime'] for entry in data_environment['luminosity']]  # Obtendo timestamps

        timestamps = convert_to_brazil_time(timestamps)  # Convertendo para horário de Brasília

        # Atualizando os dados armazenados
        stored_data['timestamps'].extend(timestamps)
        stored_data['luminosity'].extend(luminosity_values)
        stored_data['temperature'].extend(temperature_values)
        stored_data['humidity'].extend(humidity_values)

        return stored_data  # Retornando os dados atualizados

    return stored_data  # Retornando os dados se não houver nova coleta

@app.callback(
    Output('environment-graph', 'figure'),
    Input('environment-data-store', 'data'),
    Input('data-selector', 'value')
)
def update_graph(stored_data: Dict[str, List[Any]], selected_data: str) -> Dict[str, Any]:
    """
        Atualiza o gráfico com base nos dados armazenados e na seleção do usuário.

        :param stored_data: Dados armazenados.
        :param selected_data: Métrica selecionada pelo usuário.
        :return: Figura do gráfico atualizada.
    """
    if stored_data['timestamps']:
        fig_environment = go.Figure()

        # Definindo cores para as diferentes métricas
        colors = {
            'luminosity': '#F2DA5E',
            'temperature': '#CB4C4F',
            'humidity': '#4B9CCC'
        }

        if selected_data == 'all':
            for metric in ['luminosity', 'temperature', 'humidity']:
                fig_environment.add_trace(go.Scatter(
                    x=stored_data['timestamps'],
                    y=stored_data[metric],
                    mode='lines+markers',
                    name=metric.capitalize(),
                    line=dict(color=colors[metric], width=2),
                    marker=dict(size=8, symbol='circle')
                ))
        else:
            # Adicionando a métrica selecionada
            line_color = colors[selected_data]
            fig_environment.add_trace(go.Scatter(
                x=stored_data['timestamps'],
                y=stored_data[selected_data],
                mode='lines+markers',
                name=selected_data.capitalize(),
                line=dict(color=line_color, width=2),
                marker=dict(size=8, symbol='circle')
            ))

            # Calculando a média dos valores
            average_value = sum(stored_data[selected_data]) / len(stored_data[selected_data])
            # Adicionando uma linha de média
            fig_environment.add_trace(go.Scatter(
                x=stored_data['timestamps'],
                y=[average_value] * len(stored_data[selected_data]),  # Linha horizontal na média
                mode='lines',
                name='Average',
                line=dict(color='red', width=2, dash='dash')  # Linha tracejada para a média
            ))

        # Definindo o título para o eixo y baseado no valor selecionado
        yaxis_title = {
            'luminosity': 'Luminosity',
            'temperature': 'Temperature (°C)',
            'humidity': 'Humidity (%)'
        }.get(selected_data, 'Valor')

        fig_environment.update_layout(
            title={
                'text': f'{selected_data.capitalize()} Over Time' if selected_data != 'all' else 'Environmental Data Over Time',
                'font': {'size': 24},
                'x': 0.5
            },
            xaxis_title='Timestamp',
            yaxis_title=yaxis_title,
            plot_bgcolor='#3C3D40',
            paper_bgcolor='#D7D7D9',
            font=dict(color='#222326'),
            margin=dict(l=40, r=40, t=40, b=40)
        )

        fig_environment.update_xaxes(
            tickformat='%Y-%m-%d %H:%M:%S',
            tickfont=dict(color='#222326'),
            tickangle=-45,
            title_standoff=25
        )

        return fig_environment

    return {}


if __name__ == '__main__':
    app.run_server()