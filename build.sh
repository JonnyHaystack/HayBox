#!/usr/bin/env bash

profile="$(find ./config -mindepth 1 -type d -printf "%P\n" | fzf --reverse)"

if [ -z "$profile" ]; then
  echo "No profile selected, exiting"
  exit 0
fi

pio run -e "$profile" -t upload

