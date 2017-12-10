
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_ModifySpreader]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_ModifySpreader]
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- ��ѯ����
CREATE PROC GSP_GP_ModifySpreader
	@dwUserID INT,								-- �û� I D
	@strPassword NCHAR(32),						-- �û�����
	@dwSpreader INT,					-- �Ƽ���
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��������
	DECLARE @LogonPass AS NCHAR(32)
	DECLARE @SpreaderID AS NCHAR(32)

	-- ��ѯ�û�
	SELECT @SpreaderID=SpreaderID ,@LogonPass=LogonPass FROM AccountsInfo(NOLOCK) WHERE UserID=@dwUserID

	
	DECLARE @SpreaderType AS INT
	SELECT @SpreaderType=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'SpreaderType'
	IF @SpreaderType IS NULL
	BEGIN
		set @SpreaderType = 0
	END
	
	
	IF @SpreaderType = 1
	BEGIN
		-- �����ж�
		IF @LogonPass<>@strPassword
		BEGIN
			SET @strErrorDescribe=N'�����û����벻��ȷ��������Ϣ��ѯʧ�ܣ�'
			RETURN 1
		END
		-- �Ƽ���
		IF @SpreaderID<>0
		BEGIN
			SET @strErrorDescribe=N'���Ѿ��������Ƽ��ˣ�'
			RETURN 3
		END
		
		-- ��ѯ�û�
		IF not exists(SELECT * FROM AccountsInfo WHERE UserID=@dwSpreader)
		BEGIN
			SET @strErrorDescribe=N'�Ƽ���ID�����޴��Ƽ���'
			RETURN 4
		END
		
		-- ��ѯ�û�
		IF not exists(SELECT * FROM AccountsBack WHERE UserID=@dwSpreader)
		BEGIN
			SET @strErrorDescribe=N'�Ƽ���ID�����޴��Ƽ���'
			RETURN 4
		END
	END
	
	
	UPDATE AccountsInfo SET SpreaderID=@dwSpreader
	WHERE UserID=@dwUserID
	
	SET @strErrorDescribe=N'�����Ƽ��˳ɹ�!'
	
	SELECT @dwSpreader AS SpreaderID
		
END

RETURN 0

GO
