if [ ! -d "build" ]; then
    mkdir build
fi

if [ ! -d "build/testFiles" ]; then
    mkdir build/testFiles
fi

# dd if=/dev/urandom of=build/testFiles/dataBig.bin bs=1M count=1024

randomname()
{
chars=ab23
name=""
rand=$[ $RANDOM % 4 ]
for i in {1..4} ; do
    name=$name"${chars:RANDOM%${#chars}:1}"
done
echo $name
}

cd build/testFiles

echo "${number[*]}"

for n in {1..100}; do
    filename=$(randomname)
    rand=$$RANDOM 
    content=$(randomname)
    for n in {0..5}; do
        dirs[$n]=$(randomname)
    done
    finPath="."

    for n in {1..5}; do
        if [ $[$RANDOM & n] -eq 0 ]; then
            finPath=$finPath/${dirs[$n]}
        fi
    done
    echo $finPath
    mkdir -p $finPath
    echo $content > $finPath/$filename
    # dd if=/dev/urandom of=.$finPath bs=1 count=$(( RANDOM + 1024 ))
done

cd ../..