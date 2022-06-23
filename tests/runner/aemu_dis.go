package runner

import (
	"bytes"
	"os/exec"
)

func AemuDis(path string) (string, error) {
	// TODO: Run makefile
	// TODO: Allow asan/msan/ubsan
	cmd := exec.Command(disassemble, path)
	var stdout bytes.Buffer
	cmd.Stdout = &stdout
	err := cmd.Run()
	if err != nil {
		return "", err
	}
	return stdout.String(), nil
}
