<h1 align='center'>:desktop_computer: CP5_Environment-Monitor</h1>

![banner](https://github.com/L-A-N-E/CP2_Edge_1SEM/assets/153787379/132308ff-27a0-45e7-8323-80d9103f2390)

## Índice :page_with_curl:

  * [Descrição do Projeto](#descrição-do-projeto-memo)
     * [Introdução](#introdução-memo)
     * [LDR](#ldr-bulb)
     * [DHT11/22](#dht1122-snowflake)
     * [Gráficos Dinâmicos](#gráficos-dinâmicos-black_nib)
     * [Conclusão](#conclusão-black_nib)
  * [Acesso ao projeto](#acesso-ao-projeto-file_folder)
  * [Ferramenta utilizada](#ferramenta-utilizada-hammer_and_wrench)
  * [Bibliotecas utilizadas](#bibliotecas-utilizadas-books)
  * [Componentes necessários](#componentes-necessários-toolbox)
  * [Montagem](#montagem-wrench)
     * [Cuidados durante a montagem](#cuidados-durante-a-montagem-warning)
  * [Reprodução](#reprodução-gear)
  * [Como reproduzir o Dashboard Dinâmico](#como-reproduzir-o-dashboard-dinâmico-chart_with_upwards_trend)
  * [Pessoas Desenvolvedoras do Projeto](#pessoas-desenvolvedoras-do-projeto-globe_with_meridians)

## Descrição do Projeto :memo:
### Introdução :left_speech_bubble:

<p>Desde o início, nosso objetivo na LANE foi o monitoramento eficiente do ambiente de uma adega. Compreendendo a importância de condições ideais para a conservação de vinhos e outros produtos, decidimos explorar tecnologias IoT (Internet das Coisas) que nos permitissem capturar dados essenciais sobre o ambiente. Para isso, estudamos o ESP32 e o FIWARE em nosso projeto passado, que são fundamentais para a integração de dispositivos e a coleta de dados em tempo real. </p>

### LDR :bulb:

<p>Para iniciar nosso projeto, implementamos um LDR (Light Dependent Resistor) com o propósito de medir a luminosidade dentro da adega. A luz desempenha um papel crítico na preservação de vinhos, pois a exposição inadequada pode comprometer sua qualidade. O uso do LDR nos permitiu monitorar a intensidade da luz e garantir que as condições de armazenamento fossem sempre apropriadas. Essa fase foi crucial para entender como os sensores interagem com a plataforma FIWARE, possibilitando a coleta e o processamento contínuo de dados.</p>

### DHT11/22 :snowflake:

<p>Após a implementação do LDR, avançamos para a adição do sensor DHT11/22. Este sensor é responsável por medir a umidade e a temperatura, dois fatores essenciais para a conservação adequada de produtos em uma adega. A temperatura deve ser mantida em níveis específicos para evitar a deterioração, enquanto a umidade precisa ser controlada para prevenir a secagem das rolhas. A inclusão do DHT11/22 em nosso sistema nos permitirá um monitoramento contínuo e preciso dessas variáveis, oferecendo uma gestão ainda mais eficaz do ambiente.</p>

### Gráficos Dinâmicos :bar_chart:

<p>Para complementar a captura de dados, desenvolvemos um dashboard dinâmico que apresenta em tempo real as três condições ambientais monitoradas: luminosidade, umidade e temperatura. Este painel de controle é intuitivo e interativo, permitindo que os usuários visualizem as informações de forma clara e acessível. Com gráficos e indicadores, o dashboard facilita a análise dos dados, possibilitando ajustes imediatos nas condições da adega, caso necessário. Essa solução não apenas melhora a experiência do usuário, mas também assegura que as condições de armazenamento permaneçam em níveis ideais.</p>

### Conclusão :black_nib:

<p>Com a implementação do LDR e do DHT11/22, aliada ao nosso dashboard dinâmico, estamos confiantes de que nosso sistema de monitoramento proporcionará uma gestão eficaz do ambiente da adega.</p>

## Acesso ao projeto :file_folder:

Você pode acessar o [código do projeto](cp5.ino) ou a [simulação feita no Wokwi](https://wokwi.com/projects/406654345905426433)

## Ferramenta utilizada :hammer_and_wrench:

- ``Arduino IDE``
  
## Bibliotecas utilizadas :books:

- ``WiFi``
- ``PubSubClient``
- ``DHT sensor library``
- ``LiquidCrystal_I2C``

## Componentes necessários :toolbox:

|   Componente   | Quantidade |
|:--------------:|:----------:|
|      Cabos     |     12     |
|   ESP32  |     1      |
| Módulo LDR - 4 terminais  |     1      |
| DHT11 ou 22  |     1      |
| LCD 16x2 com módulo I2C |     1      |
|    Cabo USB    |     1     |

## Montagem :wrench:

<details>
  <summary>Imagem da Montagem</summary>
  <img src="https://github.com/user-attachments/assets/3d2d14d9-4ed1-47ca-bf7c-a6a44faa2443">
</details>

<h3>Cuidados durante a montagem :warning:</h3>

- ``1.`` Conectando o Cabo USB:
  - ``1.1.``Na hora de conectar o cabo com o ESP32, é preciso ter muito cuidado, pois a solda pode descolar, então seja cuidadoso!

- ``2.`` Conectando o LDR:
  - ``2.1.`` Nesta segunda parte, mudamos o LDR para um Módulo LDR com 4 terminais no qual possui a opção da saída dos dados analógicos ou digitais. Com isso, nesse projeto, continuamos usando a entrada analógica. Então, verifique se o cabo que está conectado ao D34 do ESP32 está  conectado ao A0 do LDR.
  - ``2.2.`` Conecte o VCC no terminal positivo (3V3) e o GND no terminal negativo (GND);
  - ``2.3.`` Relaxe, um dos terminais do LDR ficará sem conectar, pois esse é onde sai os dados digitais;

- ``3.`` Conectando o DHT:
  - ``3.1.`` Na simulação e na imagem acima, utilizamos o DHT22, entretanto, no [código do projeto](cp5.ino) utilizamos o DHT11;
    - ``3.1.1.`` Esses sensores possuem a diferença de que o DHT22 é mais preciso do que o DHT11, no qual possui uma margem de erro maior;
    - ``3.1.2.`` Para mais informações sobre o DHT22, nós fizemos um [material explicando sobre o sensor](https://drive.google.com/file/d/1xqHKRi8RzEcJZDfiAaG9nQOlKiJlPApr/view?usp=sharing).
    - ``3.1.3.`` Se quiser utilizar um DHT22, a única coisa que precisará será trocar ,no código, o tipo do DHT para 22 nessa seguinte parte:
            
      ```cpp
      //Definindo o tipo do DHT  
      #define DHTTYPE DHT11   
      ```
      
  - ``3.2.`` É necessário ter muito cuidado na hora de conectar os cabos, pois se conecta-los errados, o DHT queimará. Segue a Imagem de quais são os terminais do DHT:
      <details>
        <summary>Imagem dos terminais do DHT11</summary>
        <img src="https://github.com/L-A-N-E/CP2_Edge_1SEM/assets/101829188/d26416fb-d639-4760-b590-593932e5a888" alt="Terminais do DHT11">
      </details>
  - ``3.3.`` Conecte o VCC no terminal positivo (3v3), GND no terminal negativo (GND), o terminal dos dados no pino D4 e não conecte nada no terminal N.C.;

- ``4.`` Conectando o LCD:
  - ``4.1.`` **Atenção!** Estamos utilizando um LCD 16x2 com um módulo I2C!;
  - ``4.2.`` Conecte o VCC no terminal positivo (3v3), GND no terminal negativo (GND), o SDA no pino do ESP32 D21 e o SCL no D22;
  - ``4.3.`` Teste para ver se o display está funcionando, se tiver problemas com o display, pode ser algumas dessas possibilidades: o LCD está quebrado, com mal contato ou o contraste está baixo;
    - ``4.3.1.`` Para aumentar o contraste do display basta girar o trimpot de ajuste do contraste no sentido anti-horário. Por sua vez, para diminuir o contraste gire no sentido horário.
      <details>
        <summary>Imagem de onde fica o trimpot de ajuste do contraste</summary>
        <img src="https://github.com/L-A-N-E/CP2_Edge_1SEM/assets/101829188/50648d65-2402-4508-a47d-1d38bbf663e5" alt="Ajuste Contraste">
      </details>

## Reprodução :gear:

- ``1.`` Após a montagem do projeto, é necessário inserir o código por meio de um computador que possui o programa Arduino IDE instalado;
- ``2.``Ao abrir a IDE, é necessário fazer algumas coisas para selecionar o ESP32:
  - ``2.1.`` Clique em **Arquivo** > **Preferências**.
    - ``2.1.1.`` No campo **URLs Adicionais para Gerenciadores de Placas**, adicione o seguinte link: *https://dl.espressif.com/dl/package_esp32_index.json*;
  - ``2.2.`` Clique em **Ferramentas** > **Placas** > **Gerenciar Placas**.
    - ``2.2.1.`` Na janela que se abre, digite *ESP32* na caixa de pesquisa;
    - ``2.2.2.`` Selecione a plataforma *esp32* da lista e clique em Instalar;
  - ``2.3.`` Após a instalação, vá novamente em **Ferramentas** > **Placas**.
    - ``2.3.1.`` Você verá uma nova opção para selecionar as placas ESP32. Escolha a placa específica que você está usando;
- ``3.`` Baixe as [bibliotecas necessárias](#bibliotecas-utilizadas-books) no Arduino IDE;
- ``4.`` Faça as devidas modificações no código disponível:
  
  ```cpp
    const char* default_SSID = "SUA_INTERNET"; // Nome da rede Wi-Fi 
    const char* default_PASSWORD = "SENHA_DA_SUA_INTERNET"; // Senha da rede Wi-Fi 
    const char* default_BROKER_MQTT = "IP_PÚBLICO"; // IP do Broker MQTT 
  ```

  - ``4.1.`` Substitua a "SUA_INTERNET" pelo nome de sua internet;
  - ``4.2.`` Substitua a "SENHA_DA_SUA_INTERNET" pela senha de sua internet;
  - ``4.3.`` Substitua o "IP_PÚBLICO" pelo ip do servidor do Cloud Service de sua preferência:
    - ``4.3.1.`` Não disponibilizamos o IP por motivos de segurança. Para testar este código, você precisará de um serviço de nuvem, como Azure ou AWS. Além disso, será necessário instalar o FIWARE e o Docker nesse serviço e, por fim, abrir as portas necessárias. 
- ``5.`` Transferir o código do computador para  o ESP32 por meio do Cabo USB;
- ``6.`` Teste o sistema para verificar se ele está recebendo instruções e enviando dados via Postman;
- ``7.`` Com tudo montado e pronto, é necessário levá-lo para o ambiente em que será implementado e ligá-lo á uma fonte;

<p align='center'><i>OBS: Se o ESP32 for uma versão mais antiga, pode ser necessário pressionar o botão BOOT na placa durante a transferência do código </i></p>

## Como reproduzir o Dashboard Dinâmico :chart_with_upwards_trend:

- ``1.`` Ligue a máquina virtual (VM) de sua escolha e abra a porta ``8050``.
- ``2.`` Abra o terminal Linux.
- ``3.`` Siga os passoas a seguir:
  - ``3.1.`` Crie uma pasta e entre dentro dela:
    ```bash
      mkdir dashboard
      cd dashboard
    ```
  - ``3.2.`` Crie um arquivo python e dentro do arquivo copie o [código em python](dashboard/main.py) disponível neste repositório:
    - ``3.2.1.`` No arquivo, substitua o ``IP_ADDRESS = "PUBLIC_IP"``  pelo IP do servidor do Cloud Service de sua preferência.
      
    ```bash
      nano dashboard.py
    ```
  
  - ``3.4.`` Crie uma pasta assets e entre dentro dela:
    
    ```bash
      mkdir assets
      cd assets
    ```
  
  - ``3.5.`` Crie um arquivo css e dentro do arquivo copie o [código](dashboard/assets/styles.css) disponível neste repositório:
    
    ```bash
      nano styles.css
    ```
  
- ``4.`` Após os passos a cima, instale as depêndencias (como o Dash):

   ```bash
      cd ..
      pip install dash
    ```

- ``4.`` Inicie o código em Python:

   ```bash
      python3 dashboard.py
    ```

- ``5.`` Por fim, abra seu navegador e pesquise: ``PUBLIC_IP:8050``

## Pessoas Desenvolvedoras do Projeto :globe_with_meridians:

| [<img src="https://avatars.githubusercontent.com/u/101829188?v=4" width=115><br><sub>Alice Santos Bulhões</sub>](https://github.com/AliceSBulhoes) |  [<img src="https://avatars.githubusercontent.com/u/163866552?v=4" width=115><br><sub>Eduardo Oliveira Cardoso Madid</sub>](https://github.com/EduardoMadid) |  [<img src="https://avatars.githubusercontent.com/u/148162404?v=4" width=115><br><sub>Lucas Henzo Ide Yuki</sub>](https://github.com/LucasYuki1) | [<img src="https://avatars.githubusercontent.com/u/153787379?v=4" width=115><br><sub>Nicolas Haubricht Hainfellner</sub>](https://github.com/NicolasHaubricht) |
| :---: | :---: | :---: | :---: |
| RM:554499 | RM:556349 | RM:554865 | RM:556259 |

