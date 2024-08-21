# Subdomain Enumeration Tool

This C program automates the process of discovering subdomains, filtering live domains, and running further reconnaissance using various tools like `assetfinder`, `subfinder`, `httpx`, `waybackurls`, `dnsx`, and `naabu`. The program runs these tools in parallel where possible, saving time and effort.

## Features

- **Parallel Execution**: The program executes `assetfinder` and `subfinder` simultaneously to discover subdomains quickly.
- **Live Domain Checking**: Uses `httpx` to check if the discovered subdomains are live.
- **Further Reconnaissance**:
  - `waybackurls` is used to fetch URLs from the Wayback Machine.
  - `naabu` is used to perform port scanning on live domains.
- **Time-stamped Logging**: The tool logs the start time of each task, allowing for easy tracking of process execution.

## Prerequisites

Make sure you have the following tools installed on your system:

- `assetfinder`
- `subfinder`
- `httpx`
- `waybackurls`
- `dnsx`
- `naabu`

These tools can be installed via `go` or your package manager. For example:

```bash
go install -v github.com/tomnomnom/assetfinder@latest
go install -v github.com/projectdiscovery/subfinder/v2/cmd/subfinder@latest
go install -v github.com/projectdiscovery/httpx/cmd/httpx@latest
go install -v github.com/tomnomnom/waybackurls@latest
go install -v github.com/projectdiscovery/dnsx/cmd/dnsx@latest
go install -v github.com/projectdiscovery/naabu/v2/cmd/naabu@latest
```


