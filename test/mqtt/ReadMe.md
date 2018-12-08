This contains some windows batch files to send out MQTT messages to the keezer for testing purposes.  Since the MQTT broker needs a user name password you will need to copy the config.sample.ini to config.ini and edit the file with the IP, user name and password for the broker.  This keeps the secrets out of source control.

These test tools are dependent on python and paho-mqtt, so make sure you have python installed and also install paho-mqtt with the following command:
```
pip install paho-mqtt
```

monitor.py will monitor a topic in a continuous loop and output the topic and payload to the console when one is received. You can call this python script from a windows command line as follows:
```
python monitor.py -c "config.ini" -t /home/#
```

publish.py will publish a given message on the given topic. You can call this python script from a windows command line as follows:
```
python publish.py -c "config.ini" -t /home/keezer/ping -m " "
```

monitor.bat is an example windows batch file that calls the monitor.py passing in some valid parameters. 
