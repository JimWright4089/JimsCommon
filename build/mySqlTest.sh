#!/bin/bash

valgrind --tool=memcheck --leak-check=full -s ./mySQLTest ~/.local/connections/test_db.json