# WiFinder
The purpose of this project is to write a library which allows the end user to configure the WiFi settings for the ESP without having to hardcode the SSID and passphrase into the program.

This works by searching for WiFi networks and comparing them against known networks (storing known networks while powered off is currently left to the programmer), if a known network is found then control is return to the program and the SSID and passphrase can be returned. Otherwise, a soft AP is started with a captive portal, which the user should connect to. There is drop down box with the scanned SSID's and a passphrase input. Once this user has chosen the network and inputted the password the soft AP is closed and the data returned to the main program.

# Currently working
Including this code with a known SSID and passphrase is no worse than not including it at all.
  - Finds and stores SSID's
  - Opens AP if none known

# To do
  - Does not reliably send users to captive portal
  - Does not receive post request with SSID/passphrase
  - Occasionally crashes and restarts from InstrFetchProhibited on core 0 (WiFi current spike causing trip?)
