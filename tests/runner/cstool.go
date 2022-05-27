package runner

import (
	"bytes"
	"os/exec"
)

func CsTool(hex string) string {
	cmd := exec.Command("cstool", "arm", hex)
	var stdout bytes.Buffer
	cmd.Stdout = &stdout
	err := cmd.Run()
	if err != nil {
		panic(err)
	}
	return stdout.String()
}
