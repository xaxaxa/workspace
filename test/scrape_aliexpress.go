package main

import (
	"fmt"
	"io"
	"os"
	"strings"

	"golang.org/x/net/html"
)

var noClosingTags = map[string]bool {
	"input": true,
	"link": true,
}


func warn(msgs ...interface{}) {
	fmt.Println(append([]interface{}{"[warn]"}, msgs...)...)
}
func main() {
	z := html.NewTokenizer(os.Stdin)

	// stack of StartTagToken's
	var elementStack []html.Token

	for {
		tt := z.Next()
		t := z.Token()

		// keep track of the element stack
		if tt == html.ErrorToken {
			e := z.Err()
			if e == io.EOF {
				return
			}
			warn("parser error: ", e.Error())
		} else if tt == html.StartTagToken {
			_, exists := noClosingTags[t.Data]
			if !exists {
				elementStack = append(elementStack, t)
			}
		} else if tt == html.EndTagToken {
			l := len(elementStack)
			if l <= 0 {
				warn("end tag", t.Data, "encountered without matching starting tag")
				continue
			}
			prev := elementStack[l-1]
			if prev.Data != t.Data {
				warn("incorrect end tag", t.Data, "encountered; should be", prev.Data)
				continue
			}
			// pop
			elementStack = elementStack[0 : l-1]
		}

		// application specific parsing logic
		if tt == html.StartTagToken {
			attrs := make(map[string]string)
			isProductLine := false
			for _, attr := range t.Attr {
				k := strings.ToLower(attr.Key)
				attrs[k] = attr.Val

				if k == "class" {
					if strings.Index(attr.Val, "product") != -1 {
						isProductLine = true
					}
				}
			}
			if isProductLine {
				fmt.Println("product:", attrs["title"])
			}
		}
	}

}
