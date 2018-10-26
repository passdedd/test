[toc]



#### pcie
```
 在dts里面enable pci
soc上电之后会初始化pci控制器，然后我们往往会看到link down或是link up 的打印
link up是检测到了外挂的pci设备，这是系统起来之后输入lspci，会看到pci设备。 
salvator:/data # lspci                                                         
00:00.0 Class 0604: 1912:0025
01:00.0 Class 0280: 14e4:43ef
如果是link down，此时设备控制器driver会初始化失败，去检查外挂设备是否上电，
```
#### uboot下烧写分区
```
1. Set current mmc device
=> mmc dev 0
2. Get mmc partion infomation
=> mmc part
Part    Start LBA       End LBA         Name
        Attributes
        Type GUID
        Partition GUID
  1     0x00000400      0x00000bff      "bootloader"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   21200400-0804-0146-9dcc-a8c51255994f
  2     0x00000c00      0x00000fff      "misc"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   d117f98e-6f2c-d04b-a5b2-331a19f91cb2
  3     0x00001000      0x000013ff      "pst"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   25718777-d0ad-7443-9e60-02cb591c9737
  4     0x00001400      0x000017ff      "vbmeta_a"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   8a4bb8b4-e304-ae48-8536-aff5c9c495b1
  5     0x00001800      0x00001bff      "vbmeta_b"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   08992135-13c6-084b-9322-3391ff571e19
  6     0x00001c00      0x00001fff      "dtb_a"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   333a128e-d3e3-b94d-92f4-d3ebd9b3224f
  7     0x00002000      0x000023ff      "dtb_b"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   f20aa902-1c5d-294a-9177-97a513e3cae4
  8     0x00002400      0x0000a3ff      "boot_a"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   db88503d-34a5-3e41-836d-c757cb682814
  9     0x0000a400      0x000123ff      "boot_b"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   012b3303-34ac-284d-99b4-34e03a2335f4
 10     0x00012400      0x0001a3ff      "metadata"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   faec2ecf-8544-e241-b19d-757e796da607
 11     0x0001a400      0x0047e3ff      "system_a"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   f13a0978-b1b5-1a4e-8821-39438e24b627
 12     0x0047e400      0x008e23ff      "system_b"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   b710eb04-45b9-e94a-8d0b-21458d596f54
 13     0x008e2400      0x009623ff      "vendor_a"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   ad5ec4b6-2d9f-8544-9417-fe3bd1c9b1b3
 14     0x00962400      0x009e23ff      "vendor_b"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   cceb0b18-39cb-d547-9db7-03b405fa77d4
 15     0x009e2400      0x03a3dfde      "userdata"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   d4981a2b-0478-544e-9607-7fd3c651068d

3. Get block address, block count and memory size in hexdecimal (set boot_b as an example)
--------------------------------------------------------------------- 
 9     0x0000a400      0x000123ff      "boot_b"
        attrs:  0x0000000000000000
        type:   ebd0a0a2-b9e5-4433-87c0-68b6b72699c7
        guid:   012b3303-34ac-284d-99b4-34e03a2335f4

start_block = a400
end_block = 123ff
block_count = end_block - start_block + 1 = 8000
size = block_count * block_size = 1000000
--------------------------------------------------------------------- 

4. Read image to memory
usb start;mw.b 40000000 0xff 1000000;fatload usb 0:1 40000000 boot.img
usb start;mw.b 40000000 0xff 1000000;fatload usb 0:1 40000000 dtb.img

5. write data to emmc partions
mmc dev 0;mmc write 40000000 2400 8000
mmc dev 0;mmc write 40000000 1c00 400
setenv bootmode android;boota 0 boot 0x4C000000
```
#### wpa_supplicant使用方法
```
wpa_supplicant -Dnl80211 -iwlan0  -c/data/wpa_supplicant.conf&
```
#### wpa_cli使用方法
* 加密

```
wpa_cli -iwlan0 -p/data/misc/wifi/sockets
add_network      (It will display a network id for you, assume it returns 0)
set_network 0 ssid “666”
set_network 0 psk “11111111”
enable_network 0
quit
```
* 不加密

```
wpa_cli -iwlan0 -p/data/misc/wifi/sockets
add_network      (It will display a network id for you, assume it returns 0)
set_network 0 ssid “666”
set_network 0 key_mgmt NONE
enable_network 0
quit
```
#### renesas pinctrl

```
renesas 的pin function配置是通过设置groups和function来配置的，groups和function的配置是在./drivers/pinctrl/sh-pfc/pfc-r8a7795.c文件中成组配置的，
但是有的gpio在dts中像这样配置的gpio = <&gpio3 5 GPIO_ACTIVE_HIGH>;如果复用了就会导致配置groups和function时候失败
```
#### 加载driver时需要依赖其他模块

```
加载driver是需要依赖其他模块 可以在probe 时候 判定依赖文件，如果不存在 return -EPROBE_DEFER; ，这样driver隔段时间会重新probe
```
#### rsync

```
rsync -lrP hl@192.168.104.51:/sda2/qcom12.1/ /你的目录
```
#### pm
```
pm package list 
```

#### nfs

```
sudo apt install nfs-common
sudo apt-get install nfs-server
sudo apt-get install rpcbind
sudo vim /etc/exports
add  /sda2/nfs/ *(rw,sync,no_root_squash)
sudo /etc/init.d/nfs-kernel-server restart
```

```
ssh-keygen -t ed25519 -b 256 -f /etc/ssh/ssh_host_ed25519_key
ssh-keygen -t ecdsa -b 256 -f /etc/ssh/ssh_host_ecdsa_key
ssh-keygen -t ecdsa -b 1024 -f /etc/ssh/ssh_host_ecdsa_key
ssh-keygen -t dsa -b 1024 -f /etc/ssh/ssh_host_dsa_key
ssh-keygen -t rsa -b 2048 -f /etc/ssh/ssh_host_rsa_key
```

#### Ubuntu下禁止自动打开U盘等设备

```
打开终端
禁止自动挂载：
$ gsettings set org.gnome.desktop.media-handling automount false
禁止自动挂载并打开
$ gsettings set org.gnome.desktop.media-handling automount-open false
允许自动挂载
$ gsettings set org.gnome.desktop.media-handling automount true
允许自动挂载并打开
$ gsettings set org.gnome.desktop.media-handling automount-open true
```
#### uart 只能发不能收

```
原因可能是ｕａｒｔ　连接的别的ｉｃ　会把ｒｘ一直拉高，调试的时候，最好先量一下ＴＸ，ＲＸ脚，以确保无外界芯片干扰
```

#### modetest

```
modetest -M virtio_gpu -s 30:640x480 -C
```
