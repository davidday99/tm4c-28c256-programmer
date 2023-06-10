file=$1
address=$2

filesz=$(stat --printf=%s $file)

echo "Writing $file at address $address"

# (sudo stty raw speed 115200 >&2; echo $address ) >/dev/ttyACM0 <&1

offset=0

while [ $filesz -gt 0 ]
do
    if [ $filesz -lt 1024 ]
    then
        writecount=$filesz
    else
        writecount=1024
    fi

    (sudo stty raw speed 115200 >&2; echo "WRITE" | unix2dos) >/dev/ttyACM0 <&1
    (sudo stty raw speed 115200 >&2; echo $address | unix2dos) >/dev/ttyACM0 <&1
    (sudo stty raw speed 115200 >&2; echo $writecount |unix2dos) >/dev/ttyACM0 <&1
    (sudo stty raw speed 115200 >&2; echo $(dd if=$file bs=1 count=$writecount skip=$offset 2> /dev/null)| unix2dos) >/dev/ttyACM0 <&1

    offset=$(($offset + $writecount))
    address=$((address + $writecount))
    filesz=$(($filesz - $writecount))
    echo "offset: $offset"
    echo "address: $address"
done
