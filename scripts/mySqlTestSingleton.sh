#!/bin/bash

valgrind --tool=memcheck --leak-check=full -s ./mySqlSingletonTest ~/.local/connections/test_db.json