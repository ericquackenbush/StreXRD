#!/bin/bash

for f in *.mar3450
do
	./header $f
	g=${f%.*}
	h="$g.img"
	./readmar345 $f -o $h
done
