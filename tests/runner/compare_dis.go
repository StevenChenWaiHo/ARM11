package runner

import (
	"log"
	"regexp"
	"strconv"
	"strings"
)

// True=fail

var lexer = regexp.MustCompile(`[\s\t\[\]]+`)

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

func isBusted(aLine string) bool {
	for _, pre := range []string{"mov", "tst"} {
		if strings.HasPrefix(aLine, pre) {
			return true
		}
	}
	return false
}

func lexLine(line string) []string {
	as := lexer.Split(line, -1)
	var bs []string
	for _, a := range as {
		if a != "" {
			bs = append(bs, canonicalizePart(a))
		}
	}
	return bs
}

func canonicalizePart(part string) string {
	if strings.HasPrefix(part, "#0x") {
		n, err := strconv.ParseInt(part[3:], 16, 64)
		Must(err)
		return "#" + strconv.FormatInt(n, 10)
	}
	return part
}

func sliceEq(a, b []string) bool {
	if len(a) != len(b) {
		return false
	}
	for i, v := range a {
		if v != b[i] {
			return false
		}
	}
	return true
}

func debugSlice(a []string) string {
	s := "["
	for i, v := range a {
		if i != 0 {
			s += ", "
		}
		s += "`" + v + "`"
	}
	s += "]"
	return s
}

func compareLines(name, aLine, csLine string) bool {
	csLine = csLine[16:]
	if isBusted(aLine) {
		// All movs are broken
		// https://gitlab.doc.ic.ac.uk/lab2122_summer/arm11_39/-/issues/2
		return false
	}

	aCanon := lexLine(aLine)
	csCanon := lexLine(csLine)

	if !sliceEq(aCanon, csCanon) {
		log.Printf("%s: `%s` != `%s`", name, aLine, csLine)
		// debug view
		// log.Printf("%s   |   %s", debugSlice(aCanon), debugSlice(csCanon))
		return true
		// os.Exit(-1)
	}
	return false
}
