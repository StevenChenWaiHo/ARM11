package runner

import (
	"log"
	"strings"
)

// True=fail

func CompareDis(name, aemuDis, csDis string) {
	aLines := strings.Split(aemuDis, "\n")
	for i, csLine := range strings.Split(csDis, "\n") {
		// println(i, csLine)
		aLine := aLines[i*2+1]
		if compareLines(name, aLine, csLine) {
			return
		}
	}
}

func compareLines(name, aLine, csLine string) bool {
	csLine = csLine[16:]
	if aLine != csLine {
		log.Printf("%s: `%s` != `%s`", name, aLine, csLine)
		return true
		// os.Exit(-1)
	}
	return false
}
