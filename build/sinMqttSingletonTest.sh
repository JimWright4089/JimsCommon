#!/bin/bash

valgrind --tool=memcheck --leak-check=full -s ./sinMqttSingletonTest ~/.local/connections/mqtt.json