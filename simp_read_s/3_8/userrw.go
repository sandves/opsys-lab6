package main

import (
	"flag"
	"fmt"
	"log"
	"os"
)

func main() {
	msg := flag.String("write", "", "message to write to /dev/simp_rw")
	flag.Parse()

	if *msg != "" {
		write(*msg)
	} else {
		read()
	}
}

func write(msg string) {
	file, err := os.OpenFile("/dev/simp_rw", os.O_RDWR, 0644)
	defer file.Close()
	checkError(err)
	_, err = file.Write([]byte(msg))
	checkError(err)
}

func read() {
	file, err := os.Open("/dev/simp_rw")
	defer file.Close()
	checkError(err)
	data := make([]byte, 100)
	count, err := file.Read(data)
	checkError(err)
	fmt.Printf("%q\n", data[:(count-1)])
}

func checkError(err error) {
	if err != nil {
		log.Fatal(err)
	}
}
