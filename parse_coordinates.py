import sys

def parse_line(s):
    contents = s.split("$")
    for i in range(1, len(contents)):
        if contents[i].split(",")[0] == "GPGGA":
            return parse_gpgga(contents[i])
        else:
            return []

def parse_gpgga(s):
    contents = s.split(",")
    # Format: latitude, latitude direction, longitude, longitude direction, elevation, number of satellites, hdop
    out = [contents[2], contents[3], contents[4], contents[5], contents[9], contents[7], contents[8]]
    out[0] = convert_coordinates(out[0])
    out[2] = convert_coordinates(out[2])
    return out

def convert_coordinates(c):
    return int(c.split(".")[0][:-2]) + float(c.split(".")[0][-2:] + "." + c.split(".")[1]) / 60

def create_gpx(l):
    out = ['<?xml version="1.0" encoding="UTF-8"?>']
    out.append('<gpx version="1.1" creator="Jan">')
    out.append("<trk>")
    out.append("<name>Drive Track</name>")
    out.append("<trkseg>")

    for e in l:
        out.append('<trkpt lat="{}" lon="{}">'.format(e[0], e[2]))
        out.append("<ele>{}</ele>".format(e[4]))
        out.append("<sat>{}</sat>".format(e[5]))
        out.append("<hdop>{}</hdop>".format(e[6]))
        out.append("</trkpt>")

    out.append("</trkseg>")
    out.append("</trk>")
    out.append("</gpx>")

    return out

def parse_file(name):
    file = open(name)
    coordinates = []
    for line in file:
        l = parse_line(line)
        if l != [] and l != None:
            coordinates.append(l)
    print("".join(create_gpx(coordinates)))
    with open("out.gpx", 'w') as fp:
        for item in create_gpx(coordinates):
            fp.write("%s\n" % item)

if __name__ == "__main__":
    parse_file(sys.argv[1])
