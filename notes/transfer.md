### 文件传输（利用管道，不使用`rx sx`传输文件）

1. 使用管道并不会加快速度，串口的波特率无法满足“高速”传输。
2. 使用管道受串口电磁干扰的影响更大，请使用`md5sum`手动检查。
3. 使用 [tio/tio: A simple serial device I/O tool](https://github.com/tio/tio) 的`Xmodem-1K`已解决我的问题。

---

出于一些还没搞明白的问题，我的 ArchLinux 就算是在安装了`extra/lrzsz`后仍旧无法通过`minicom`在串口传输文件。
尝试了安装 `busybox` 使用 `busybox sx`，问题也并未解决。

上述种种让我放弃了 `rx sx` 的传输方案。我的笔记本电脑没有网口，所以通过桥接传输也就此告吹（太麻烦）。

- https://github.com/esp8266/source-code-examples/issues/26

在配置 `minicom` 的时候，获得了 `/dev/ttyUSB0` 的权限。

```console
sudo usermod -a -G uucp "$(whoami)" # Arch
# Warning : `logout` and `login` is needed
```

那么，在 `UNIX` 世界中，是否可以通过管道 `|` 将文件流重定向到 `/dev/ttyUSB0` 中呢？答案是，可行

- https://unix.stackexchange.com/questions/460342/retrieve-file-over-serial-without-kermit-and-lrzsz

```console
# -------- minicom(GEC-6818) --------      # -------- Host (Arch Linux) --------
cat | base64 -d > {filename}
                                           cat {filename} | base64 > /dev/ttyUSB0
# Ctrl + D (which means `EOF`)
```

本人目前并未有从开发板传回文件的需求，此处未列出，但可参考上方链接实现。

### USB 挂载传输大文件

USB 挂载，使用 `cp` 传输文件。
