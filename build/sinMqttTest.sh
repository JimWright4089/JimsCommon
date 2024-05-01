#!/bin/bash

valgrind --tool=memcheck --leak-check=full -s ./sinMqttTest ~/.local/connections/mqtt.json