
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_GetExchangeHuaFei]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].GSP_GP_GetExchangeHuaFei
GO


IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_ExchangeDone]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].GSP_GP_ExchangeDone
GO


SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- ��ѯ����
CREATE PROC GSP_GP_GetExchangeHuaFei
	
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

SELECT * FROM ExchangeHuaFeiList
		
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------
-- ִ�жһ�
CREATE PROC GSP_GP_ExchangeDone
	@dwUserID 		INT,								-- �û� I D
	@dwExchangeID 	INT,								-- �һ� I D
	@strPassword	NCHAR(32),				-- �����Ϣ
	@strErrorDescribe NVARCHAR(127) OUTPUT				-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��ѯ�û�
	DECLARE @LogonPassword NCHAR(32) 
	SELECT @LogonPassword=LogonPass FROM AccountsInfo WHERE UserID=@dwUserID
	IF @LogonPassword IS NULL
	BEGIN
		SET @strErrorDescribe = N'�����û���Ϣ������,�������ǵĿͷ���Ա��ϵ��'
		return 1				
	END

	-- У������
	IF @LogonPassword <> @strPassword
	BEGIN
		SET @strErrorDescribe = N'���ĵ�¼���벻ƥ�䣡'
		return 2		
	END
	
	DECLARE @UseType int	--1��� 2שʯ 3���ѿ�
	DECLARE @UseNum int
	DECLARE @GetType int	
	DECLARE @GetNum int
	
	SELECT @UseType = UseType,@UseNum = UseNum,@GetType = GetType,@GetNum = GetNum FROM ExchangeHuaFeiList WHERE ExchangeID=@dwExchangeID
	
	if @UseType is NULL
	BEGIN
		set @strErrorDescribe = N'û���ҵ�����Ʒ'
		return 1
	END

	DECLARE @InsureScore bigint
	DECLARE @Score bigint
	SELECT @Score = Score,@InsureScore = InsureScore FROM QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo WHERE UserID=@dwUserID
	
	if @Score is NULL
	BEGIN
		set @strErrorDescribe = N'û���ҵ����'
		return 1
	END
	
	if @UseType = 1
	BEGIN
		if @Score < @UseNum
		BEGIN
			set @strErrorDescribe = N'��Ľ�Ҳ���'
			return 1
		END
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score = Score - @UseNum WHERE UserID = @dwUserID
	END
	
	if @UseType = 2
	BEGIN
		if @InsureScore < @UseNum
		BEGIN
			set @strErrorDescribe = N'�����ʯ����'
			return 1
		END
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET InsureScore = InsureScore - @UseNum WHERE UserID = @dwUserID
	END
	
	
	if @GetType = 1
	BEGIN
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score = Score + @GetNum WHERE UserID = @dwUserID
	END
	
	if @GetType = 2
	BEGIN
		UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET InsureScore = InsureScore + @GetNum WHERE UserID = @dwUserID
	END
	
	set @strErrorDescribe = N'�һ��ɹ�'
	RETURN 0
END

RETURN 0

GO