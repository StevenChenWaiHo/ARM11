package main

import (
	"io/ioutil"
	"log"
	"os"
	"path"

	"gitlab.doc.ic.ac.uk/lab2122_summer/arm11_39/tests/runner"
)

func main() {
	log.SetFlags(0)
	gotoRootDir()

	paths, err := ioutil.ReadDir("tests/dis")
	runner.Must(err)

	// cstool bless
	/*for _, p := range paths {
		if !p.IsDir() && path.Ext(p.Name()) == "" {
			f, err := os.Open("tests/dis/" + p.Name())
			runner.Must(err)
			bytes, err := ioutil.ReadAll(f)
			runner.Must(err)
			hexStr := hex.EncodeToString(bytes)
			csDis := runner.CsTool(hexStr)
			err = os.WriteFile("tests/dis/"+p.Name()+".csdis", []byte(csDis), 0644)
			runner.Must(err)
		}
	}*/

	// aemu bless
	for _, p := range paths {
		if !p.IsDir() && path.Ext(p.Name()) == "" {
			aemuDis, err := runner.AemuDis("tests/dis/" + p.Name())
			if err != nil {
				log.Printf("AEMU FAIL %s", p.Name())
			} else {
				runner.Must(os.WriteFile("tests/dis/"+p.Name()+".aemudis", []byte(aemuDis), 0644))
			}
		}
	}
}

func gotoRootDir() {
	for {
		if runner.DirExists("tests") && runner.DirExists("arm11_testsuite") && runner.DirExists("src") {
			return
		}
		cwd, err := os.Getwd()
		runner.Must(err)
		if cwd == "/" {
			log.Fatalf("Could not find project root directory")
		}
		runner.Must(os.Chdir(".."))
	}
}
