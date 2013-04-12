#!/bin/sh
exec ls "$(echo "$QUERY_STRING"|sed -f urldecode.sed)"

