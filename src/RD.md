# poison

## Graphviz
Install ```graphviz``` package to convert dot files to png/jpeg images.

### Generating png files from dot files
It is recommended that you use neato instead of dot to obtain more human readable graphs. Use the following command to generate png
image file from a dot file:
```bash
neato -Tpng <input_dot_file> -o <output_png_file>
```
