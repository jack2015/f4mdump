F4MDump
Info:
Simple F4M/HDS (Adobe HTTP Dynamic Streaming) dumper. Intended to be used by IPTVPlayer plugin: https://gitorious.org/iptv-pl-dla-openpli, http://iptvplayer.vline.pl.

For downloading data with URL syntax F4MDump do not use any library it use wget, called as child process using vfor/execvp.

Usage:
List available bitrates from manifest file: F4MDump "/path/to/wget with additional params if needed" "http://url.to/Manifest.f4m"

Downloading: F4MDump "/path/to/wget with additional params if needed" "http://url.to/Manifest.f4m" "/path/to/out/file" [bitrate]