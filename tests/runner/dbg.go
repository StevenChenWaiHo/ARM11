package runner

import (
	"bytes"
	"encoding/json"
	"io"
	"os"
	"os/exec"
	"path"
	"time"
)

type DbgSpec struct {
	File   string
	Steps  []DbgStep
	Stderr string
}

func (d *DbgSpec) Eq(other *DbgSpec) bool {
	if d.File != other.File {
		return false
	}
	if d.Stderr != other.Stderr {
		return false
	}
	if len(d.Steps) != len(other.Steps) {
		return false
	}
	for i, ds := range d.Steps {
		if ds != other.Steps[i] {
			return false
		}
	}
	return true
}

type DbgStep struct {
	Input  string
	Output string
}

func RunDbg(jsonPath string) (DbgSpec, error) {
	var spec DbgSpec
	f, err := os.ReadFile(jsonPath)
	if err != nil {
		return DbgSpec{}, err
	}
	err = json.Unmarshal(f, &spec)
	if err != nil {
		return DbgSpec{}, err
	}

	asmPath := path.Join(path.Dir(jsonPath), spec.File)

	cmd := exec.Command("./src/build/bin/debug", asmPath)
	var stderr bytes.Buffer
	cmd.Stderr = &stderr
	// stdout, err := cmd.StdoutPipe()
	// Must(err)
	var stdout bytes.Buffer
	cmd.Stdout = &stdout

	stdin, err := cmd.StdinPipe()
	Must(err)
	cmd.Start()
	oldLen := 0
	for i, step := range spec.Steps {
		writeAll(stdin, []byte(step.Input))
		writeAll(stdin, []byte("\n"))
		time.Sleep(time.Millisecond * 10) // TODO: Be better
		out := stdout.String()
		spec.Steps[i].Output = out[oldLen:]
		oldLen = len(out)
	}
	cmd.Wait()
	spec.Stderr = stderr.String()
	return spec, nil
}

func writeAll(w io.Writer, b []byte) {
	n, err := w.Write(b)
	if n != len(b) {
		panic(err)
	}
}
