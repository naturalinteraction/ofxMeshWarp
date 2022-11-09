make -j8

zip "pollex.zip" bin/example bin/data/gradient1.png bin/data/gradient1reversed.png
aws s3 mv "pollex.zip" s3://natural-interaction/files/development/