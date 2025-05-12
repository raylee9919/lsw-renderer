vim.api.nvim_exec([[
augroup HeaderAndSourceFileTemplate
autocmd!
autocmd BufNewFile *.h :lua require'file_templates'.insert_header_template()
autocmd BufNewFile *.c,*.cpp :lua require'file_templates'.insert_sourcefile_template()
augroup END
]], false)

local module = {}

function module.insert_header_template()
    local filename_upper = string.upper(vim.fn.expand('%:t:r'))
    local current_year = os.date("%Y")
    local header_template = string.format([[
#ifndef %s_H_
#define %s_H_
/* ========================================================================

   (C) Copyright %s by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */




#endif // %s_H_]], filename_upper, filename_upper, current_year, filename_upper)

    vim.api.nvim_buf_set_lines(0, 0, 0, false, vim.split(header_template, '\n'))
    vim.fn.cursor(10, 0)
end

function module.insert_sourcefile_template()
    local filename = vim.fn.expand('%:t')
    local current_year = os.date("%Y")
    local sourcefile_template = string.format([[
/* ========================================================================

   (C) Copyright %s by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */




    ]], current_year)

    vim.api.nvim_buf_set_lines(0, 0, 0, false, vim.split(sourcefile_template, '\n'))
    vim.fn.cursor(6, 0)
end

return module
