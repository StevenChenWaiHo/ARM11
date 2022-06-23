package main

import (
	"bytes"
	"encoding/hex"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"path"
	"strings"
	"time"

	"gitlab.doc.ic.ac.uk/lab2122_summer/arm11_39/tests/runner"
)

var bless bool = false
var nTest int = 0
var failed bool = false

func main() {
	log.SetFlags(0)
	gotoRootDir()

	_, bless = os.LookupEnv("AEMU_BLESS")

	start := time.Now()
	testDis()
	testAsmPass()
	taken := time.Since(start)
	fmt.Printf("Ran %d tests in %s\n", nTest, taken)
	if failed {
		fmt.Print("==== FAILED ====\n")
	}
}

func testDis() {
	paths, err := ioutil.ReadDir("tests/dis")
	runner.Must(err)

	if bless {
		// cstool bless
		for _, p := range paths {
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
		}

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
	} else {
		for _, p := range paths {
			if !p.IsDir() && path.Ext(p.Name()) == "" {
				aemuDis, err := os.ReadFile("tests/dis/" + p.Name() + ".aemudis")
				if err != nil {
					log.Printf("Fail to get output for `%s`", p.Name())
					continue
				}
				csDis, err := os.ReadFile("tests/dis/" + p.Name() + ".csdis")
				if err != nil {
					log.Printf("CS FAIL %s", p.Name())
					continue
				}
				nTest++
				runner.CompareDis(p.Name(),
					strings.TrimSpace(string(aemuDis)),
					strings.TrimSpace(string(csDis)))
				fmt.Printf("PASS dis/%s\n", p.Name())
			}
		}
	}
}

func testAsmPass() {
	paths, err := ioutil.ReadDir("tests/asm-pass")
	runner.Must(err)
	for _, p := range paths {
		if !p.IsDir() && path.Ext(p.Name()) == ".s" {
			pname := "tests/asm-pass/" + p.Name()

			asmed, err := runner.RunAsm(pname)
			if err != "" {
				println(err)
				failed = true
				continue
			}
			outFile := withExt(pname, ".out")
			if bless {
				os.WriteFile(outFile, asmed, 0o644)
			} else {
				expected, err := os.ReadFile(outFile)
				runner.Must(err)
				nTest++
				if bytes.Equal(asmed, expected) {
					fmt.Printf("PASS asm-pass/%s\n", pname)
				} else {
					fmt.Printf("FAIL asm-pass/%s\n", pname)
				}
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

func withExt(name, ext string) string {
	oldExt := path.Ext(name)
	base := name[:len(name)-len(oldExt)]
	return base + ext
}
