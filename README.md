# ESPHome Configuration

### This project represents my ESPHome configuration, which I use to control various smart devices in my home.

This ESPHome configuration is organized into different YAML files, each corresponding to a specific device or component.

Devices typically include YAML files from other locations in this project.  My goal was to reuse configuration where possible.  An example of this is the 

### Secrets Configuration

To use this configuration, you will need to create the `secrets.yaml` file.  This file contains sensitive information such as Wi-Fi credentials and API keys. Make sure to keep this file secure and do not share it publicly.  You will notice that file is included in the project `.gitignore` file, so it will not be uploaded to GitHub.  Some of the required fields in the `secrets.yaml` file include:
- `wifi_ssid`: Your Wi-Fi network name.
- `wifi_password`: Your Wi-Fi network password.
- `mqtt_broker`: The address of your MQTT broker.

Here is an example of how to structure the `secrets.yaml` file:

```yaml
# Network details
wifi_ssid:          "Your_Wi-Fi_SSID"
wifi_password:      "Your_Wi-Fi_Password"
wifi_gateway:       192.168.1.1
wifi_subnet:        255.255.255.0
wifi_dns:           192.168.1.2
broker_address:     192.168.1.3
wifi_ssid_backup:   "Your_backup_Wi-Fi_SSID"
wifi_ssid_hotspot:  "YourPhoneHotspot"
wifi_pass_hotspot:  "YourPhoneHotspotPassword"
default_password:   "YourSecurePassword"

# Device API keys and Over-The-Air passwords
air_quality_api:    "ATqypySkOEFnjKeq2Y05WFag4RQE2DMe/+qdFZjWSWU="
air_quality_ota:    "cfa6437354f34fe7d44fed8a6e0c8800"
```
Change those values to match your own network and device configurations.  Do not use any of the example passwords provided above, as they will have been scanned by bots and are not secure.  Always use strong, unique passwords for your devices and network.

Once you have set up the `secrets.yaml` file, you can use the device YAML files in this repository to configure your ESPHome devices accordingly. Each YAML file corresponds to a specific device or group of devices, and you can customize them based on your needs.  Make sure to follow the ESPHome documentation for any specific configurations or integrations you want to implement.

### Note

Because git on Homeassistant has limited merge conflict handling, the repo can be reset by using the following command in the terminal:

```bash
git reset --hard origin/main
```

This will reset your local repository to match the remote repository, discarding any local changes! Use this command with caution, as it will permanently delete any uncommitted changes in your local repository.
