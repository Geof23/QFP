CREATE OR REPLACE FUNCTION public.createschmoo(run integer,
                                               prec text[],
                                               compilers text[],
                                               tests text[],
					       optls text[],
                                               fname text
)
 RETURNS text
 LANGUAGE plpython3u
AS $function$
from plpy import spiexceptions
from sys import path
path.append('/home/sawaya/temp/qfp/db/python')
import plotting as pl

prec_str = ""
if len(prec) > 0:
   prec_str = " and (precision = '"
   for t in prec:
      prec_str += t + "' or precision = '"
   prec_str = prec_str[:-17] + ")"

optl_str = ""
if len(optls) > 0:
   optl_str = " and (optl = '"
   for t in optls:
      optl_str += t + "' or optl = '"
   optl_str = optl_str[:-12] + ")"

tests_str = ""
if len(tests) > 0:
   tests_str = " and (position('"
   for t in tests:
      tests_str += t + "' in name) > 0 or position('"
   tests_str = tests_str[:-13] + ")"

comp_str = ""
if len(compilers) > 0:
   comp_str = " and (compiler = '"
   for c in compilers:
      comp_str += c + "' or compiler = '"
   comp_str = comp_str[:-16] + ")"
   
quer = ("select distinct switches, compiler, optl, precision " +
        "from tests where " +
        "run = " + str(run) +
        prec_str + comp_str + optl_str + tests_str +
        " UNION " + 
        "select distinct switches, compiler, optl, precision " +
        "from tests where " +
        "run = " + str(run) +
        prec_str + comp_str + tests_str + " and switches = ''" +
        " and optl = '-O0'" +
        " order by compiler, optl, switches")
x_axis = plpy.execute(quer)
xa_count = len(x_axis)

quer = ("select distinct name from tests where run = " + str(run) +
        prec_str + tests_str + comp_str + " order by name")

y_axis = plpy.execute(quer)
ya_count = len(y_axis)
x_ticks = []
y_ticks = []
z_data = []

x_count = 0
y_count = 0

for x in x_axis:
   x_ticks.append(x['switches'] + ' ' +
                  x['optl'])
for t in y_axis:
    y_ticks.append(t['name'])
    y_count += 1
    quer = ("select distinct score0 from tests where run = " + str(run) +
            " and name = '" + t['name'] + "'" + prec_str + comp_str +
            tests_str + " order by score0")
    scores = plpy.execute(quer)
    eq_classes = {}
    line_classes = []
    for x in range(0, len(scores)):
       eq_classes[scores[x]['score0']] = x
    for x in x_axis:
        quer = ("select score0 from tests where name = '" +
                t['name'] + "' and precision = '" + x['precision'] +
                "' and switches = '" + x['switches'] +
                "' and compiler = '" + x['compiler'] +
                "' and optl = '" + x['optl'] + "' and run = " + str(run))
        score = plpy.execute(quer)
        x_count += 1
        line_classes.append(eq_classes[score[0]['score0']])
    z_data.append(line_classes)

pl.plot(x_ticks, y_ticks, z_data, fname, ', '.join(compilers) +
        ' @ precision(s): ' +
        ', '.join(prec))

$function$
