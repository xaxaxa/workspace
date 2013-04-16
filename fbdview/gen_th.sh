#!/bin/bash
gen_th() {
	ls|grep images.0|while read x; do
		convert -resize 144x144 "$x" "$x".th;
	done
};
gen_th;
