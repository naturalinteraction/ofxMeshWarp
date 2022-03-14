# ./version.sh
# make -j8

# rm bin/lib*.so

ls -ls bin
echo "press any key to distribute or control-c to abort"

while [ true ] ; do
read -t 3 -n 1
if [ $? = 0 ] ; then
break ;
fi
done

zip "pollex.zip" -r bin/example
# install*.sh
aws s3 mv "pollex.zip" s3://natural-interaction/files/