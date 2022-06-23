package runner

import (
	"bytes"
	"fmt"
	"os"
	"os/exec"
)

func RunAsm(path string) ([]byte, string) {
	tmp, err := os.CreateTemp("", "aemu-out")
	Must(err)
	defer os.Remove(tmp.Name())

	cmd := exec.Command("./src/build/bin/assemble", path, tmp.Name())
	var stdout bytes.Buffer
	var stderr bytes.Buffer
	cmd.Stdout = &stdout
	cmd.Stderr = &stderr
	err = cmd.Run()
	if err != nil {
		return nil, fmt.Sprintf("asm `%s` failed: %s, \nstderr: `%s`\nstdout: `%s`", path, err.Error(), stderr.String(), stdout.String())
	}

	asm, err := os.ReadFile(tmp.Name())
	Must(err)
	return asm, ""
}
