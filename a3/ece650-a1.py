import sys
import re

# YOUR CODE GOES HERE
class Vertex(object):

    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

        self.point = '(' + str(x) + ',' + str(y) + ')'
        self.point_list = []

    def __repr__(self):
        return '({0:.2f},{1:.2f})'.format(self.x, self.y)


class Line(object):
    def __init__(self, p1, p2):
        self.src = p1
        self.dst = p2

    def __repr__(self):
        return repr(self.src) + '-->' + repr(self.dst)


def intersect(l1, l2):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    x_num = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))
    x_den = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))

    y_num = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)
    y_den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)

    if x_den != 0 and y_den != 0:
        x_coor = x_num / x_den
        y_coor = y_num / y_den

        if min(x1, x2) <= x_coor <= max(x1, x2) and min(y1,
                                                        y2) <= y_coor <= max(
            y1, y2):
            if min(x3, x4) <= x_coor <= max(x3, x4) and min(
                    y3, y4) <= y_coor <= max(y3, y4):
                return Vertex(x_coor, y_coor)
    else:
        return None


def ontheline(i, j, q):
    if (q.x - i.x) * (j.y - i.y) == (j.x - i.x) * (q.y - i.y) and min(i.x, j.x) <= q.x <= max(i.x, j.x) and min(i.y,
                                                                                                                j.y) <= q.y <= max(
        i.y, j.y):
        return 1
    else:
        pass


def get_streetname(command):
    try:
        cmd = command.split('"')
        # print(cmd[1])
        if len(cmd) >= 1:

            street_name = cmd[1]

            for x in str(street_name):
                if (not x.isalpha()) and (not x.isspace()):
                    print('Error: the format of input is wrong, please check the street name.')
        else:
            return

        return street_name

    except:
        IndexError


def get_point(command):
    point_list = []
    line_list = []
    try:

        cmd = command.split('"')

        vertex_list = cmd[2]

        pattern = re.compile(r'\((\-?\d+),(\-?\d+)\)')

        points = pattern.findall(vertex_list)

        for p in points:
            v = Vertex(p[0], p[1])

            if v not in point_list:
                point_list.append(v)

        if len(point_list) < 2:
            print('Error: please input at least two points')

        return point_list


    except:
        IndexError


def get_line(points):
    try:
        line_list = []
        # print(point_list)

        for i in range(len(get_point(points)) - 1):
            l = Line(get_point(points)[i], get_point(points)[i + 1])

            line_list.append(l)
        return line_list

    except:
        TypeError

    # print(line_list)


def add_street(command):
    # r = re.compile(r'a \"(.+?)\"(( ?\(\-?\d+,\-?\d+\))+)\s*$')
    # v = r.findall(command)
    # print (v)

    pattern = r'a \"(.+?)\"(( ?\(\-?\d+,\-?\d+\))+)\s*$'

    if re.match(pattern, command):

        street_name = get_streetname(command)

        point_list = get_line(command)


    else:
        print('Error: The format of input is wrong.')


# def check_street():


def change_street(command):
    pattern = r'c \"(.+?)\"(( ?\(\-?\d+,\-?\d+\))+)\s*$'

    if re.match(pattern, command):

        cmd = command.split('"')
        # print(cmd[2])

        street_name = cmd[1]

        vertex_list = cmd[2]

    else:
        print('Error: The format of input is wrong.')


def generate(streets):
    #v_id = 0
    v_id = -1;
    vertices = {}
    vertices_list = []

    # print(streets)
    if streets != {}:
        street_name = list(streets.keys())  # python3
        edge_list = []

        intersection_list = []
        intersection_dict = {}

        for i in range(len(street_name) - 1):
            for j in range(i + 1, len(street_name)):
                s1 = streets[street_name[i]]
                s2 = streets[street_name[j]]
                # print('compare')

                for l1 in s1:
                    for l2 in s2:
                        intersection = intersect(l1, l2)
                        # print(intersection)

                        # add the endpoints of lines have intersections
                        if intersection is not None:
                            # print(l1, l2)
                            intersection_dict.setdefault(intersection, []).append(l2)

                            intersection_dict.setdefault(intersection, []).append(l1)

                            if not str(intersection) in intersection_list:
                                intersection_list.append(intersection)
                            else:
                                pass

                            endpoint1 = l1.dst
                            endpoint2 = l1.src
                            endpoint3 = l2.dst
                            endpoint4 = l2.src

                            # print(endpoint1, endpoint2, endpoint3, endpoint4)

                            endpoint_list = [endpoint1, endpoint2, endpoint3, endpoint4]

                            for p in endpoint_list:

                                if str(p) in vertices_list:
                                    pass
                                else:
                                    # print(intersection)
                                    v_id = v_id + 1
                                    vertices[v_id] = p
                                    vertices_list.append(str(p))

                            # add the intersections
                            if str(intersection) in vertices_list:
                                pass
                            else:
                                # print(intersection)
                                v_id = v_id + 1
                                vertices[v_id] = intersection
                                vertices_list.append(str(intersection))

        sys.stdout.write('V {}\n'.format(len(vertices_list)))
        # print(len(vertices_list))
        sys.stdout.flush()

        # generate edges
        for i in range(len(intersection_list) - 1):

            for j in range(i + 1, len(intersection_list)):

                if intersection_list[j].x < intersection_list[i].x:

                    c = intersection_list[j]

                    intersection_list[j] = intersection_list[i]

                    intersection_list[i] = c

                elif intersection_list[j].x == intersection_list[i].x:
                    if intersection_list[j].y < intersection_list[i].y:
                        c = intersection_list[j]

                        intersection_list[j] = intersection_list[i]

                        intersection_list[i] = c

        for k in range(len(street_name)):
            s = streets[street_name[k]]

            for line in s:
                end1 = line.src
                end2 = line.dst
                if end1.x > end2.x:

                    c = end1

                    end1 = end2

                    end2 = c

                elif end1.x == end2.x:
                    if end1.y > end2.y:
                        c = end1

                        end1 = end2

                        end2 = c

                else:

                    pass

                # print (end1)

                print_list = []

                for inter in intersection_list:

                    # print(line)
                    i_value = intersection_dict[inter]

                    # print(end1)

                    for value in i_value:

                        if value == line:

                            if str(end1) not in print_list:
                                print_list.append(str(end1))

                            if str(inter) not in print_list:
                                print_list.append(str(inter))

                if str(end2) not in print_list:
                    print_list.append(str(end2))

                # print(print_list)

                for m in range(len(print_list) - 1):
                    l = Line(print_list[m], print_list[m + 1])
                    # print(l)
                    if str(l) not in edge_list:
                        edge_list.append(l)

        # for key, value in vertices.items():
        #     index_list.append(key)

        # print('index',index_list)

        # def get_index(vertices, value):
        #     return (k for k, v in vertices.items() if v == value)

        new_vertices = {str(v): k for k, v in vertices.items()}
        # print(new_vertices)

        # print('edges', edge_list)
        # print('E = {')
        sys.stdout.write('E {')
        sys.stdout.flush()

        cnt = 0
        for edge in edge_list:
            index_a = new_vertices[edge.src]
            index_b = new_vertices[edge.dst]
            cnt = cnt + 1
            if cnt < len(edge_list):
                sys.stdout.write(str('<') + str(index_a) + str(',') + str(index_b) + str('>') + str(','))
                sys.stdout.flush()
            else:
                sys.stdout.write(str('<') + str(index_a) + str(',') + str(index_b) + str('>'))
                sys.stdout.flush()

        sys.stdout.write('}\n')
        sys.stdout.flush()
        v_id = 0

    else:
        #print('Error: the set is empty')
        vertices.clear()
        del vertices_list[:]



    # print(street_name)
    # index_list = []


def main():
    ### YOUR MAIN CODE GOES HERE

    ### sample code to read from stdin.

    ### make sure to remove all spurious print statements as required

    ### by the assignment

    streets = {}

    while True:
        try:
            command = raw_input('').lower()  # python3 input python2 raw_input

            key = get_streetname(command)

            init_streets = {}

            if command == '':
                break

            elif command[0] == 'a':
                # print('add')
                # add_street(command)
                # create_dataset(command)
                if key in streets:  ## if streets.has_key(key) in python2+
                    print('Error: this street is already exist, try again.')

                else:
                    # print(key)
                    value = get_line(command)

                    init_streets[key] = value

                    streets.update(init_streets)
                #print(streets)



            elif command[0] == 'c':
                # print('change')

                if key in streets:  ## if streets.has_key(key) in python2+

                    value = get_line(command)
                    # print(value)
                    init_streets[key] = value

                    streets.update(init_streets)

                    # change_street(command)
                else:
                    print('Error: this street does not exsit, try again')

                # print(streets)


            elif command[0] == 'r':
                # print('remove')
                if key in streets:  ## if streets.has_key(key) in python2+

                    del streets[key]
                    streets.update()

                else:
                    print('Error: this street does not exsit, try again')

                # print(streets)

            elif command[0] == 'g':
                # print('generate')
                if streets != {}:
                    generate(streets)
                else:
                    sys.stdout.write('V 0\n')
                    sys.stdout.flush()

                    sys.stdout.write('E {}\n')

                    sys.stdout.flush()
            else:
                print('Error: The format of input is wrong.')
            sys.stdout.flush()

        # return exit code 0 on successful termination
        except EOFError:
            sys.exit(0)
            # print('Finished reading input')


if __name__ == '__main__':
    main()
