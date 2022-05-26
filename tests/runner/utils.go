package runner

import "os"

func Must(err error) {
	if err != nil {
		panic(err)
	}
}

func DirExists(name string) bool {
	s, err := os.Stat(name)
	if err != nil {
		return false
	}
	return s.IsDir()
}
