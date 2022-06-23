package main

import (
	"bytes"
	"encoding/hex"
	"encoding/json"
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
	testAsmFail()
	testDbg()
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
				pname := "tests/dis/" + p.Name()
				aemuDis, err := os.ReadFile(pname + ".aemudis")
				if err != nil {
					log.Printf("Fail to get output for `%s`", p.Name())
					continue
				}
				csDis, err := os.ReadFile(pname + ".csdis")
				if err != nil {
					log.Printf("CS FAIL %s", p.Name())
					continue
				}
				nTest++
				runner.CompareDis(pname,
					strings.TrimSpace(string(aemuDis)),
					strings.TrimSpace(string(csDis)))
				fmt.Printf("PASS %s\n", pname)
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
					fmt.Printf("PASS %s\n", pname)
				} else {
					fmt.Printf("FAIL %s\n", pname)
					failed = true
				}
			}
		}
	}
}

func testAsmFail() {
	paths, err := ioutil.ReadDir("tests/asm-fail")
	runner.Must(err)
	for _, p := range paths {
		if !p.IsDir() && path.Ext(p.Name()) == ".s" {
			pname := "tests/asm-fail/" + p.Name()
			stderr, didFail := runner.RunAsmFail(pname)
			nTest++
			if !didFail {
				failed = true
				fmt.Printf("FAIL %s: Exited sucess\n", pname)
				continue
			}

			stderrName := withExt(pname, ".stderr")
			if bless {
				os.WriteFile(stderrName, []byte(stderr), 0o644)
			} else {
				exprected, err := os.ReadFile(stderrName)
				runner.Must(err)
				if bytes.Equal(exprected, stderr) {
					fmt.Printf("PASS %s\n", pname)
				} else {
					failed = true
					fmt.Printf("FAIL %s\n", pname)
				}
			}
		}
	}
}

func testDbg() {
	paths, err := ioutil.ReadDir("tests/dbg")
	runner.Must(err)
	for _, p := range paths {
		if !p.IsDir() && path.Ext(p.Name()) == ".json" {
			pname := "tests/dbg/" + p.Name()
			dbgOut, err := runner.RunDbg(pname)
			if err != nil {
				panic(err)
			}
			nTest++
			if bless {
				out, err := json.MarshalIndent(dbgOut, "", "  ")
				runner.Must(err)
				runner.Must(os.WriteFile(pname, out, 0o644))
			} else {
				expectedJson, err := os.ReadFile(pname)

				runner.Must(err)
				var expected runner.DbgSpec
				runner.Must(json.Unmarshal(expectedJson, &expected))
				if expected.Eq(&dbgOut) {
					fmt.Printf("PASS %s\n", pname)
				} else {
					failed = true
					fmt.Printf("FAIL %s\n", pname)
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
