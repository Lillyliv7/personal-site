cd ..
echo "Cleaning www"
rm -rf www
mkdir www
mkdir log
mkdir www/css
mkdir www/res
mkdir www/js
echo "Compiling Typescript files in ./typescript to ./www/js"
tsc --project ./tsconfig.json
echo "Uglifying Javascript files with Yuicompressor"
# find ./www/js -name "*.js" -size +0c -exec yuicompressor -v {} -o {} \;
echo "Uglifying CSS files with Yuicompressor"
# cp -r css/. www/css/
echo "Copying misc files"
cp -r otherWWW/. www/
find ./css -name "*.css" -exec sh -c 'yuicompressor -v "$0" -o "./www/css/$(basename "$0")"' {} \;
echo "Uglifying HTML files"
html-minifier --collapse-whitespace --remove-comments --remove-optional-tags --remove-redundant-attributes --remove-script-type-attributes --remove-tag-whitespace --use-short-doctype --minify-css true --minify-js true --input-dir ./html --output-dir ./www
cp -r ./res/* ./www/res
exit
