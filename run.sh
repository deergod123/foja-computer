#!/bin/bash
gnome-terminal -x bash -c "cd server; node server.js; exec $SHELL"
gnome-terminal -x bash -c "cd src; yarn start; exec $SHELL"
