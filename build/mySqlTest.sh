#!/bin/bash

valgrind --tool=memcheck --leak-check=full -s ./mySqlTest ~/.local/connections/test_db.json