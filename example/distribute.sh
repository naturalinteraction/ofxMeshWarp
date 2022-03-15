make -j8

zip "pollex.zip" -r bin/example
# install*.sh
aws s3 mv "pollex.zip" s3://natural-interaction/files/