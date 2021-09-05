#!/bin/bash

get_filename () {
  local file=$1
  local page=${file##*/}
  local name=${page%.*}
  echo "$name"
}

cd src

for page in ../pages/*.html; do
  name=$(get_filename $page)

  ls ../site/themes | sort -R | tail -$N | while read file; do
    theme=$(get_filename $file)
    echo "page: $name, theme: $theme"
    ./generate $name $theme
    break
  done
done

cd ..
