$PBExportHeader$fvs.sra
$PBExportComments$Generated Application Object
forward
global type fvs from application
end type
global transaction sqlca
global dynamicdescriptionarea sqlda
global dynamicstagingarea sqlsa
global error error
global message message
end forward

global type fvs from application
string appname = "fvs"
end type
global fvs fvs

on fvs.create
appname = "fvs"
message = create message
sqlca = create transaction
sqlda = create dynamicdescriptionarea
sqlsa = create dynamicstagingarea
error = create error
end on

on fvs.destroy
destroy( sqlca )
destroy( sqlda )
destroy( sqlsa )
destroy( error )
destroy( message )
end on

