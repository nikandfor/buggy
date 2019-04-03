#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <unistd.h>

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

int parse(const char *name, int *var) {
    int r = sscanf(optarg, "%d", var);
    if (r != 1) {
        fprintf(stderr, "%s parse error\n", name);
        return 1;
    }
    return 0;
}

int parseArgs0(int argc, char *argv[], int *w, int *h, int *x, int *y) {
    int opt;
    while ((opt = getopt(argc, argv, "w:h:x:y:")) != -1) {
        switch (opt) {
            case 'w':
                if (parse("width", w)) {
                    return 1;
                }
                break;
            case 'h':
                if (parse("wheel base", h)) {
                    return 1;
                }
                break;
            case 'x':
                if (parse("link x", x)) {
                    return 1;
                }
                break;
            case 'y':
                if (parse("link y", y)) {
                    return 1;
                }
                break;
            default:
                fprintf(stderr, "unexpected arg: %s\n", optarg);
                return 1;
        }
    }

    return 0;
}

int parseArgs(int argc, char *argv[], const char *ops, int *vars[]) {
    int opt;
    int i, j;
    while ((opt = getopt(argc, argv, ops)) != -1) {
        i = 0;
        for (j = 0; ops[j]; j++) {
            if (ops[j] == ':') {
                continue;
            }
            if (ops[j] == opt) {
                break;
            }
            i++;
            if (!vars[i]) {
                fprintf(stderr, "unexpected arg (%c): %s\n", opt, optarg);
                return 1;
            }
        }

        int r = sscanf(optarg, "%d", vars[i]);
        if (r != 1) {
            fprintf(stderr, "parse error: %c %s\n", opt, optarg);
            return 1;
        }
    }

    return 0;
}

double check_diff_angle(int w, int h, int x, int y, double a) {
    float l = 2 * (w + x);
    glm::vec2 rp(w, h), lp(-w, h);

    // fprintf(stderr, "l %.0lf\n", l);

    float rad = glm::radians(a);

    glm::vec2 r_link(x, y);
    glm::vec2 r_link_rot = glm::rotate(r_link, rad);

    // fprintf(stderr, "r_link rot by %.0lf: %.0lf %.0lf\n", a, r_link_rot.x, r_link_rot.y);

    glm::vec2 r_n = glm::rotate(glm::vec2(-1, 0), rad);
    float r_c = rp.x - r_n.x * rp.y / r_n.y;

    // fprintf(stderr, "r_c %.0lf\n", r_c);

    glm::vec2 l_n = glm::vec2(r_c, 0);
    // fprintf(stderr, "l_n %.0lf %.0lf\n", l_n.x, l_n.y);
    // glm::vec2 d = l_n - lp;
    // fprintf(stderr, "l_n-lp %.0lf %.0lf\n", d.x, d.y);
    l_n = glm::normalize(l_n - lp);

    // fprintf(stderr, "l_n %.0lf %.0lf\n", l_n.x, l_n.y);

    float l_ang = glm::angle(glm::vec2(-1, 0), l_n);

    // fprintf(stderr, "l_ang %.2lf\n", glm::degrees(l_ang));

    glm::vec2 l_link(-x, y);

    glm::vec2 l_link_rot = glm::rotate(l_link, l_ang);

    float dist = glm::distance(lp + l_link_rot, rp + r_link_rot);

    fprintf(stderr, "l len %.0lf  dist %.0lf   diff %.0lf\n", l, dist, fabs(l - dist));

    float r_link_c = rp.x - r_link.x * rp.y / r_link.y;

    fprintf(stderr, "link X %.0lf\n", r_link_c);

    return fabs(l - dist);
}

int main(int argc, char *argv[]) {
    int w = -1, h = -1;
    int x = -1, y = -1;
    int ang = 10;

    int *arr[] = {&w, &h, &x, &y, &ang, NULL};
    if (parseArgs(argc, argv, "w:h:x:y:a:", arr)) return 1;

    // fprintf(stderr, "wh %d %d  xy %d %d\n", w, h, x, y);

    if (w < 0 || h < 0) {
        fprintf(stderr, "whxy - are required args\n");
    }

    float ll = glm::length(glm::vec2(x, y));
    if (ll < 50 || ll > 150) {
        printf("%d\n", 100);
        return 0;
    }

    w = w / 2;

    double d = check_diff_angle(w, h, x, y, ang);

    printf("%lf\n", d);

    return 0;
}
