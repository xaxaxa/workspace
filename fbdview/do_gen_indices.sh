#!/bin/bash
ls -d *.photos *.albums *.tagged *.feed| while read x; do
	pushd "$x";
	fbdview_gen_index;
	popd;
done
