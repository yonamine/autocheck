set(SOURCES
  "autosar_rules.py"
  "genhtml.py"
)

set(TEMPLATES
  "directory_report.html.template"
  "file_report.html.template"
  "file_table_row.html.template"
  "legend_row.html.template"
  "path_link_segment.html.template"
  "path_segment.html.template"
  "pie_chart_empty.html.template"
  "pie_chart.html.template"
  "rule_table_empty.html.template"
  "rule_table_row.html.template"
  "style.css"
)

set(GENHTML_LIBDIR ${CMAKE_INSTALL_LIBDIR}/autocheck/autocheck-genhtml)

foreach(Template ${TEMPLATES})
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/templates/${Template} DESTINATION ${GENHTML_LIBDIR}/templates)
endforeach()
foreach(Source ${SOURCES})
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${Source} DESTINATION ${GENHTML_LIBDIR})
endforeach()
install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/autocheck-genhtml.py DESTINATION ${CMAKE_INSTALL_BINDIR} RENAME autocheck-genhtml)
