package main

import "os"
import "fmt"
import "log"

func main() {
	file, err := os.Open("/dev/simp_read")
	if err != nil {
		log.Fatal(err)
	}
	data := make([]byte, 100)
	count, err := file.Read(data)
	if err != nil {
		log.Fatal(err)
	}
	//The minus 1 is to get rid of the trailing \n
	fmt.Printf("%q\n", data[:(count-1)])
}
