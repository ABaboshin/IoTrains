#include <Arduino.h>
#include "OTA.h"

#ifdef ESP32
#include "esp_ota_ops.h"
#include "esp_http_client.h"

static void http_cleanup(esp_http_client_handle_t client)
{
  esp_http_client_close(client);
  esp_http_client_cleanup(client);
}

#define BUFFSIZE 1024 * 50
static char ota_write_data[BUFFSIZE + 1] = {0};
#endif

OTA::OTA(const std::string &id) : BaseDevice(id)
{
  railschema::Capability otaCapability;
  otaCapability.type = railschema::CapabilityType::OTA;
  capabilities.push_back(otaCapability);
}

std::shared_ptr<railschema::Event> OTA::Loop()
{
  return nullptr;
}

void OTA::DefaultAction()
{
}

std::shared_ptr<railschema::State> OTA::ProcessCommand(std::shared_ptr<railschema::Command> command)
{
  auto state = std::make_shared<railschema::State>();
  state->id = this->id;

  auto otaCommand = (railschema::OtaCommand *)command.get();
  state->command = *otaCommand;

  state->ok = false;

#ifdef ESP32
  esp_http_client_config_t config = {
      .url = otaCommand->url.c_str(),
      .timeout_ms = 100,
      .keep_alive_enable = true,
  };

  config.skip_cert_common_name_check = true;

  esp_http_client_handle_t client = esp_http_client_init(&config);
  if (client == NULL)
  {
    Serial.println("fail 1");
    // delay(100000);
    return state;
  }

  const esp_partition_t *configured = esp_ota_get_boot_partition();
  const esp_partition_t *running = esp_ota_get_running_partition();

  auto err = esp_http_client_open(client, 0);
  if (err != ESP_OK)
  {
    Serial.printf("Failed to open HTTP connection: %s", esp_err_to_name(err));
    Serial.println("");
    // delay(100000);
    return state;
  }
  esp_http_client_fetch_headers(client);

  auto update_partition = esp_ota_get_next_update_partition(NULL);

  esp_ota_handle_t update_handle = 0;

  int binary_file_length = 0;
  /*deal with all receive packet*/
  bool image_header_was_checked = false;
  while (1)
  {
    int data_read = esp_http_client_read(client, ota_write_data, BUFFSIZE);
    if (data_read < 0)
    {
      Serial.printf("Error: SSL data read error");
      Serial.println("");
      http_cleanup(client);
      // delay(100000);
      return state;
    }
    else if (data_read > 0)
    {
      if (image_header_was_checked == false)
      {
        esp_app_desc_t new_app_info;
        if (data_read > sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t) + sizeof(esp_app_desc_t))
        {
          // check current version with downloading
          memcpy(&new_app_info, &ota_write_data[sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t)], sizeof(esp_app_desc_t));
          Serial.printf("New firmware version: %s", new_app_info.version);
          Serial.println("");

          esp_app_desc_t running_app_info;
          if (esp_ota_get_partition_description(running, &running_app_info) == ESP_OK)
          {
            Serial.printf("Running firmware version: %s", running_app_info.version);
            Serial.println("");
          }

          const esp_partition_t *last_invalid_app = esp_ota_get_last_invalid_partition();
          esp_app_desc_t invalid_app_info;
          if (esp_ota_get_partition_description(last_invalid_app, &invalid_app_info) == ESP_OK)
          {
            Serial.printf("Last invalid firmware version: %s", invalid_app_info.version);
            Serial.println("");
          }

          // check current version with last invalid partition
          if (last_invalid_app != NULL)
          {
            if (memcmp(invalid_app_info.version, new_app_info.version, sizeof(new_app_info.version)) == 0)
            {
              Serial.printf("New version is the same as invalid version.");
              Serial.println("");
              Serial.printf("Previously, there was an attempt to launch the firmware with %s version, but it failed.", invalid_app_info.version);
              Serial.println("");
              Serial.printf("The firmware has been rolled back to the previous version.");
              Serial.println("");
              http_cleanup(client);
              // delay(100000);
              return state;
            }
          }
          // #ifndef CONFIG_EXAMPLE_SKIP_VERSION_CHECK
          //           if (memcmp(new_app_info.version, running_app_info.version, sizeof(new_app_info.version)) == 0)
          //           {
          //             ESP_LOGW(TAG, "Current running version is the same as a new. We will not continue the update.");
          //             http_cleanup(client);
          //             infinite_loop();
          //           }
          // #endif

          image_header_was_checked = true;

          err = esp_ota_begin(update_partition, OTA_WITH_SEQUENTIAL_WRITES, &update_handle);
          if (err != ESP_OK)
          {
            Serial.printf("esp_ota_begin failed (%s)", esp_err_to_name(err));
            Serial.println("");
            http_cleanup(client);
            esp_ota_abort(update_handle);
            // delay(100000);
            return state;
          }
          ESP_LOGI(TAG, "esp_ota_begin succeeded");
        }
        else
        {
          Serial.printf("received package is not fit len");
          Serial.println("");
          http_cleanup(client);
          esp_ota_abort(update_handle);
          // delay(100000);
          return state;
        }
      }
      err = esp_ota_write(update_handle, (const void *)ota_write_data, data_read);
      if (err != ESP_OK)
      {
        http_cleanup(client);
        esp_ota_abort(update_handle);
        // delay(100000);
        return state;
      }
      binary_file_length += data_read;
      Serial.printf("Written image length %d", binary_file_length);
      Serial.println("");
    }
    else if (data_read == 0)
    {
      /*
       * As esp_http_client_read never returns negative error code, we rely on
       * `errno` to check for underlying transport connectivity closure if any
       */
      if (errno == ECONNRESET || errno == ENOTCONN)
      {
        Serial.printf("Connection closed, errno = %d", errno);
        Serial.println("");
        break;
      }
      if (esp_http_client_is_complete_data_received(client) == true)
      {
        Serial.printf("Connection closed");
        Serial.println("");
        break;
      }
    }
  }
  Serial.printf("Total Write binary data length: %d", binary_file_length);
  Serial.println("");
  if (esp_http_client_is_complete_data_received(client) != true)
  {
    Serial.printf("Error in receiving complete file");
    Serial.println("");
    http_cleanup(client);
    esp_ota_abort(update_handle);
    // delay(100000);
    return state;
  }

  err = esp_ota_end(update_handle);
  if (err != ESP_OK)
  {
    if (err == ESP_ERR_OTA_VALIDATE_FAILED)
    {
      Serial.printf("Image validation failed, image is corrupted");
      Serial.println("");
    }
    else
    {
      Serial.printf("esp_ota_end failed (%s)!", esp_err_to_name(err));
      Serial.println("");
    }
    http_cleanup(client);
    // delay(100000);
    return state;
  }

  err = esp_ota_set_boot_partition(update_partition);
  if (err != ESP_OK)
  {
    Serial.printf("esp_ota_set_boot_partition failed (%s)!", esp_err_to_name(err));
    Serial.println("");
    http_cleanup(client);
    // delay(100000);
    return state;
  }
  Serial.printf("Prepare to restart system!");
  Serial.println("");
  esp_restart();
  return state;
#endif

  state->ok = true;

  return state;
}
