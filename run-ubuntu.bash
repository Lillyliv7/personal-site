echo "Cleaning www"
rm -rf www
mkdir www
mkdir www/css
mkdir www/res
mkdir www/js
echo "Compiling Typescript files in ./typescript to ./www/js"
tsc --project ./tsconfig.json
echo "Uglifying Javascript files with Yuicompressor"
find ./www/js -name "*.js" -size +0c -exec yui-compressor -v {} -o {} \;
echo "Uglifying CSS files with Yuicompressor"
find ./css -name "*.css" -exec sh -c 'yui-compressor -v "$0" -o "./www/css/$(basename "$0")"' {} \;
echo "Uglifying HTML files"
html-minifier --collapse-whitespace --remove-comments --remove-optional-tags --remove-redundant-attributes --remove-script-type-attributes --remove-tag-whitespace --use-short-doctype --minify-css true --minify-js true --input-dir ./html --output-dir ./www
cp -r ./res/* ./www/res
echo "Starting HTTP server"
cd www
python3 -m http.server 8080
echo "Server exited"
exit