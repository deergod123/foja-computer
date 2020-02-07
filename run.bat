@echo off
title app runner
cd server
start cmd.exe /k "node server.js"
cd ../src
start cmd.exe /k "yarn start"