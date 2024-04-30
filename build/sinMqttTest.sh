#!/bin/bash

valgrind --tool=memcheck --leak-check=full -s ./SinMQTTTest ~/.local/connections/mqtt.json