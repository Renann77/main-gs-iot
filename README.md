# 🌐 SafeZone – Plataforma Inteligente de Prevenção e Resposta a Desastres Naturais

Desenvolvido pelo **Inovex Group** para o desafio **Global Solution 2025 - FIAP**, este projeto propõe uma plataforma de prevenção a desastres naturais utilizando **IoT, APIs, inteligência artificial e dashboards em tempo real**.

---

## 👨‍💻 Equipe

- **Cauan da Cruz** – RM558238  
- **Renan Dorneles** – RM557820  
- **Igor Dias** – RM555217  

---

## 🧠 Introdução

Diante do aumento da frequência de desastres naturais, o SafeZone surge como uma plataforma integrada que utiliza sensores IoT, algoritmos inteligentes e comunicação via MQTT para:
- Monitorar riscos ambientais em tempo real;
- Atuar com rapidez para alertar moradores e autoridades;
- Oferecer suporte para evacuação e ações emergenciais.

---

## 🎯 Desafio e Proposta

**Desafio:** Prevenção e resposta eficiente a desastres naturais em áreas urbanas vulneráveis.

**Proposta:**  
Criar um sistema completo, escalável e replicável que combine:
- Sensores conectados (chuva, temperatura, umidade)
- Dashboard via Node-RED
- App mobile para alertas e orientações
- API REST com persistência
- Conteinerização e integração em nuvem

---

## 👥 Público-Alvo

- Populações em áreas de risco (enchentes, calor extremo, deslizamentos)
- Defesa Civil e autoridades públicas
- Gestores de abrigos, ONGs e centros comunitários

---

## 📈 Impacto Esperado

- Redução de até **40% nos danos materiais**
- Aumento de **70% na eficiência de evacuações**
- Cobertura de até **300 mil habitantes** em cidades de médio porte

> Fontes: INPE, CEMADEN, IBGE (2023)

---

## ⚙️ Tecnologias e Arquitetura

- **ESP32 com sensores IoT**
- **MQTT com broker HiveMQ**
- **Dashboard Node-RED**
- **Spring Boot + JWT (API Java)**
- **React Native (app mobile)**
- **Banco de dados relacional (Oracle)**
- **Docker + GitHub + Deploy Cloud**
- **Arquitetura TOGAF**

---

## 🔧 Componentes do protótipo IoT

| Sensor/Atuador | Função                       | Pino ESP32 |
|----------------|-------------------------------|------------|
| DHT22          | Temperatura e umidade         | GPIO 12    |
| LED            | Indicador de status remoto    | GPIO 15    |
| Sensor de chuva| Detecta início de precipitação| GPIO 13    |

---

## 🔗 Tópicos MQTT

- `fiap/iot/temphumi`: publicação de dados ambientais
- `fiap/iot/led`: controle remoto do LED (via dashboard/app)

### Exemplo de mensagem publicada:
```json
{
  "temperatura": 28.4,
  "umidade": 73.1,
  "status_led": "off",
  "chuva": "Chovendo"
}



![image](https://github.com/user-attachments/assets/6b000b3e-ae3c-4478-93e9-7bd5bd35b2c4)
