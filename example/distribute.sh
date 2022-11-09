make -j8

cp bin/example bin/plastico
zip "pollex.zip" update.sh bin/plastico bin/data/gradient1.png bin/data/gradient1reversed.png
aws s3 mv "pollex.zip" s3://natural-interaction/files/development/