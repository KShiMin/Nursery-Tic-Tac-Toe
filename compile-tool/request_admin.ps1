# Check for Administrator privileges
$principal = New-Object Security.Principal.WindowsPrincipal([Security.Principal.WindowsIdentity]::GetCurrent())
$adminRole = [Security.Principal.WindowsBuiltInRole]::Administrator

if (-Not $principal.IsInRole($adminRole)) {
    # Relaunch as administrator
    Start-Process "powershell.exe" -ArgumentList "-File `"$PSScriptRoot\install.sh`"" -Verb RunAs
    exit
} else {
    # Already has admin privileges
    & "$PSScriptRoot\install.sh"
}