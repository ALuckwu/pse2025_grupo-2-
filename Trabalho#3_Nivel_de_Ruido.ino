#include <WiFi.h>
#include <HTTPClient.h>
#include <driver/i2s.h>
#include <math.h>

// --- CONFIGURAÇÃO WIFI ---
const char* ssid = "S23+ de Alberto";
const char* password = "ccsb2106";

// --- CONFIGURAÇÃO ThingSpeak ---
const char* thingSpeakServer = "http://api.thingspeak.com/update";
const char* apiKey = "2DWI6UT6RF23HYAE";

// --- CONFIGURAÇÃO DE CONEXÃO COM MICROFONE
// --- I2S PINS ---
#define I2S_WS 15     // L/R clock
#define I2S_SD 32     // Serial data in
#define I2S_SCK 14    // Serial clock

#define SAMPLE_RATE 48000
#define SAMPLES 1024

#define MIC_REF_AMPL 0.00631 // 1 Pa em volts (calibrar)

float samples[SAMPLES];

// Filtro A-weighting (coeficientes para 48kHz)
class AWeightingFilter {
public:
    AWeightingFilter() {
        b0 = 0.25574113;
        b1 = -0.51148225;
        b2 = 0.25574113;
        a1 = -0.64718561;
        a2 = 0.14223739;

        x1 = x2 = y1 = y2 = 0;
    }

    float process(float x0) {
        float y0 = b0 * x0 + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;

        x2 = x1;
        x1 = x0;
        y2 = y1;
        y1 = y0;

        return y0;
    }

private:
    float b0, b1, b2, a1, a2;
    float x1, x2, y1, y2;
};

AWeightingFilter aFilter;

void setupI2S() {
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 1024,
    .use_apll = true
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_32BIT, I2S_CHANNEL_MONO);
}

void connectWiFi() {
  Serial.print("Conectando em WiFi ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha ao conectar WiFi.");
  }
}

void setup() {
  Serial.begin(115200);
  setupI2S();
  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  int32_t buffer[SAMPLES];
  size_t bytesRead;

  i2s_read(I2S_NUM_0, &buffer, SAMPLES * sizeof(int32_t), &bytesRead, portMAX_DELAY);

  float sum = 0;
  for (int i = 0; i < SAMPLES; i++) {
    float sample = (float)buffer[i] / INT32_MAX;
    float weighted = aFilter.process(sample);
    samples[i] = weighted;
    sum += weighted * weighted;
  }

  float rms = sqrt(sum / SAMPLES);
  float dbA = 20.0 * log10(rms / MIC_REF_AMPL) + 94.0; //FATOR DE CORREÇÃO DO MICROFONE UTILIZADO

  Serial.print("SPL (A-weighted): ");
  Serial.print(dbA, 2);
  Serial.println(" dB(A)");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = String(thingSpeakServer) + "?api_key=" + apiKey + "&field1=" + String(dbA, 2);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("Dados enviados. Código HTTP: %d\n", httpCode);
    } else {
      Serial.printf("Erro ao enviar dados: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("WiFi desconectado. Tentando reconectar...");
  }

  delay(20000); // ThingSpeak limita para 1 atualização a cada 15 segundos
}
