make -j8

zip "pollex.zip" bin/example
aws s3 mv "pollex.zip" s3://natural-interaction/files/development/