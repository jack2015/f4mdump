F4MDump
Info:
Simple F4M/HDS (Adobe HTTP Dynamic Streaming) dumper.
Intended to be used by IPTVPlayer plugin: http://iptvplayer.pl, http://iptvplayer.vline.pl.

This branch of F4MDump will use libcurl library to download fragments/segments.

Usage:
List available bitrates from manifest file: 
F4MDump "/path/to/wget with additional params if needed" "http://url.to/Manifest.f4m"

Downloading: 
F4MDump "/path/to/wget with additional params if needed" "http://url.to/Manifest.f4m" "/path/to/out/file" [bitrate]