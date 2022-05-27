package runner

import (
	"bytes"
	"os/exec"
)

func AemuDis(path string) (string, error) {
	// TODO: Run makefile
	// TODO: Put dis in seperate binary
	// TODO: Dont require dummy output
	cmd := exec.Command("./src/build/bin/emulate", path, "__")
	var stdout bytes.Buffer
	cmd.Stdout = &stdout
	err := cmd.Run()
	if err != nil {
		return "", err
	}
	return stdout.String(), nil
}
