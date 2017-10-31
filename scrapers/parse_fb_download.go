package main

import (
	"database/sql"
	"flag"
	"fmt"
	"log"
	"os"
	"strings"
	"time"

	_ "github.com/mattn/go-sqlite3"
	"golang.org/x/net/html"
)

var db *sql.DB
var dbPath = flag.String("db", "", "path to sqlite database")

type parserState int

const (
	findingThread parserState = iota
	parsingThread
	parsingMessage
	parsingMessageContent
	parsingMessageMisc
	parsingMessageUser
	parsingMessageDate
)

func attributeToMap(attr []html.Attribute) map[string]string {
	tmp := make(map[string]string)
	for _, item := range attr {
		tmp[strings.ToLower(item.Key)] = item.Val
	}
	return tmp
}

func initDB() {
	var err error
	db, err = sql.Open("sqlite3", *dbPath)
	if err != nil {
		log.Fatal(err)
	}

}
func addThread(subject string) int {
	_, err := db.Exec("insert into threads (subject) values (?)", subject)
	if err != nil {
		log.Fatal(err)
	}

	rows, err := db.Query("select last_insert_rowid()")
	if err != nil {
		log.Fatal(err)
	}
	if !rows.Next() {
		log.Fatal("select last_insert_rowid() returned no rows")
	}
	id := int(0)
	rows.Scan(&id)
	err = rows.Err()
	if err != nil {
		log.Fatal(err)
	}
	rows.Close()
	fmt.Println("inserted thread", id)
	return id
}
func addMessage(threadId int, from string, date string, msg string) {
	from = strings.Replace(from, "\n", " ", -1)
	date = strings.Replace(date, "\n", " ", -1)
	from = strings.TrimSpace(from)
	date = strings.TrimSpace(date)
	msg = strings.TrimSpace(msg)
	if strings.HasSuffix(date, "UTC") {
		date += "+00"
	}
	t, err := time.Parse("Monday, January 2, 2006 at 3:04pm UTC-07", date)
	if err != nil {
		log.Fatal(err)
	}
	timestr := t.Format("2006-01-02 15:04 MST")
	_, err = db.Exec("insert into messages (thread_id,date,\"from\",message) values (?,?,?,?)",
		threadId, timestr, from, msg)
	if err != nil {
		log.Fatal(err)
	}
}

func main() {
	flag.Parse()
	initDB()
	r := os.Stdin
	z := html.NewTokenizer(r)
	state := findingThread
	var stateStack []parserState
	curThreadId := -1
	curMsg := ""
	curMsgUser := ""
	curMsgDate := ""

	for {
		tt := z.Next()
		token := z.Token()
		if tt == html.ErrorToken {
			log.Println("html tokenizer error: ", token.String())
			break
		}

		if tt == html.StartTagToken {
			// save the previous state so that when we exit this element
			// we can restore the old state
			stateStack = append(stateStack, state)
		}

		attrs := attributeToMap(token.Attr)

		switch state {
		case findingThread:
			if tt != html.StartTagToken {
				goto cont
			}
			if val, ok := attrs["class"]; ok && strings.EqualFold(val, "thread") {
				state = parsingThread
			}
		case parsingThread:
			if tt == html.TextToken && curThreadId == -1 {
				curThreadId = addThread(token.Data)
			}
			if tt == html.StartTagToken {
				if val, ok := attrs["class"]; ok && strings.EqualFold(val, "message") {
					state = parsingMessage
				} else if token.Data == "p" {
					state = parsingMessageContent
					curMsg = ""
				}
			}
		case parsingMessageContent:
			if tt == html.TextToken {
				curMsg += token.Data
			}
		case parsingMessage:
			if tt == html.StartTagToken {
				if val, ok := attrs["class"]; ok && strings.EqualFold(val, "message_header") {
					state = parsingMessageMisc
					curMsgUser = ""
					curMsgDate = ""
				}
			}
		case parsingMessageMisc:
			if tt == html.StartTagToken {
				if val, ok := attrs["class"]; ok && strings.EqualFold(val, "user") {
					state = parsingMessageUser
				} else if val, ok := attrs["class"]; ok && strings.EqualFold(val, "meta") {
					state = parsingMessageDate
				}
			}
		case parsingMessageUser:
			if tt == html.TextToken {
				curMsgUser += token.Data
			}
		case parsingMessageDate:
			if tt == html.TextToken {
				curMsgDate += token.Data
			}
		}
	cont:
		if tt == html.EndTagToken {
			// pop from state stack
			savedState := stateStack[len(stateStack)-1]
			stateStack = stateStack[0 : len(stateStack)-1]
			if savedState != state {
				// what state we are exiting from
				switch state {
				case parsingThread:
					curThreadId = -1
				case parsingMessageContent:
					addMessage(curThreadId, curMsgUser, curMsgDate, curMsg)
				}
			}
			state = savedState
		}
	}
	db.Close()
}
